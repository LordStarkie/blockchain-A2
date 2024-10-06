#include "ui.h"
#include <GLFW/glfw3.h>
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "src/distributed_ledger/ledger.h"
#include <vector>
#include <string>
#include <cstdio>
#include <cstring>

int startUI() {
    // BACKEND
    Ledger ledger = Ledger::create_mock_ledger();

    // proof of authority manager
    PoA poa;

    // retrieve inventories from the ledger
    const std::vector<Inventory>& inventories = ledger.get_inventories();
    if (inventories.size() < 4) {
        fprintf(stderr, "Error: Not enough inventories to designate validators.\n");
        return 1;
    }

    // designate the first three inventories as validators
    poa.add_validator(inventories[0]);
    poa.add_validator(inventories[1]);
    poa.add_validator(inventories[2]);
    poa.list_validators();
    printf("\n");

    // init error callback
    glfwSetErrorCallback(error_callback);
    if (!glfwInit()) {
        fprintf(stderr, "Error: Failed to initialize GLFW.\n");
        return 1;
    }

    // GL+GLSL versions for Mac
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac

    // create GLFW
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Q1 - Version 1", nullptr, nullptr);
    if (window == nullptr) {
        fprintf(stderr, "Error: Failed to create GLFW window.\n");
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    // setup imgui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();

    // setup platform/renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // state variables
    bool main_menu = true;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    int selected_inventory = -1;

    // propose block window variables
    bool show_propose_block_window_flag = false;
    bool propose_success = false;
    std::string propose_message;

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // start frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // menu window
        ImGui::Begin("Menu");

        // create show inventory buttons
        for (size_t i = 0; i < inventories.size(); ++i) {
            std::string button_label = "Show Inventory " + std::to_string(i + 1);
            if (ImGui::Button(button_label.c_str())) {
                selected_inventory = static_cast<int>(i);
            }
        }

        ImGui::Separator();

        // button to open "Propose Block" window
        if (ImGui::Button("Propose Block")) {
            show_propose_block_window_flag = true;
            // reset messages when opening the window
            propose_success = false;
            propose_message = "";
        }

        ImGui::End();

        // display Inventory Table if an inventory is selected
        if (selected_inventory >= 0 && selected_inventory < static_cast<int>(inventories.size())) {
            show_inventory_as_table(inventories[selected_inventory], selected_inventory);
        }

        // display "Propose Block" window if flagged
        if (show_propose_block_window_flag) {
            show_propose_block_window(poa, ledger, show_propose_block_window_flag, propose_success, propose_message);
        }

        // rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // swap buffers
        glfwSwapBuffers(window);
    }

    // cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

void show_inventory_as_table(const Inventory &inventory, int& selected_inventory) {
    ImGui::Begin("Inventory Details");

    // inventory name
    std::string inventory_name = inventory.get_inventory_name();
    ImGui::Text("%s", inventory_name.c_str());
    ImGui::Separator();

    // create table 4 columns
    if (ImGui::BeginTable("InventoryTable", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
        ImGui::TableSetupColumn("Item ID");
        ImGui::TableSetupColumn("Item Price");
        ImGui::TableSetupColumn("Item Qty");
        ImGui::TableSetupColumn("Location");

        // row header
        ImGui::TableHeadersRow();

        // populate table
        const std::vector<Block>& blocks = inventory.get_blocks();
        for (const auto& block : blocks) {
            const Transaction& tx = block.get_transaction();

            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("%s", tx.get_item_id().c_str());

            ImGui::TableSetColumnIndex(1);
            ImGui::Text("%d", tx.get_item_price());

            ImGui::TableSetColumnIndex(2);
            ImGui::Text("%d", tx.get_item_qty());

            ImGui::TableSetColumnIndex(3);
            // Convert Location enum to string for better readability
            ImGui::Text("%s", location_to_string(tx.get_location()).c_str());
        }

        ImGui::EndTable();
    }

    // close button
    if (ImGui::Button("Close")) {
        selected_inventory = -1;
    }

    ImGui::End();
}

void show_propose_block_window(PoA &poa, Ledger &ledger, bool &show_window, bool &propose_success,
                               std::string &propose_message) {
    ImGui::Begin("Propose Block", &show_window, ImGuiWindowFlags_NoCollapse);

    // input fields
    static char item_id[128] = "";
    static int item_price = 0;
    static int item_qty = 0;
    static int location_index = 0;

    // locations
    const char* locations[] = { "A", "B", "C", "D", "E"};

    ImGui::InputText("Item ID", item_id, IM_ARRAYSIZE(item_id));
    ImGui::InputInt("Item Price", &item_price);
    ImGui::InputInt("Item Quantity", &item_qty);
    ImGui::Combo("Location", &location_index, locations, IM_ARRAYSIZE(locations));

    // submit button
    if (ImGui::Button("Submit")) {
        // validate inputs
        if (strlen(item_id) == 0) {
            propose_success = false;
            propose_message = "Item ID cannot be empty.";
        } else if (item_price <= 0) {
            propose_success = false;
            propose_message = "Item Price must be positive.";
        } else if (item_qty <= 0) {
            propose_success = false;
            propose_message = "Item Quantity must be positive.";
        } else {
            Location location = static_cast<Location>(location_index);

            // transaction
            Transaction tx(item_id, item_price, item_qty, location);

            // call propose
            bool result = poa.propose_block(tx, location, ledger);

            if (result) {
                propose_success = true;
                propose_message = "Block proposed successfully. Transaction added to all inventories.";
                // Clear input fields
                memset(item_id, 0, sizeof(item_id));
                item_price = 0;
                item_qty = 0;
                location_index = 0;
            } else {
                propose_success = false;
                propose_message = "Block proposal failed. Inventory is not a validator.";
            }
        }
    }

    // success/error message
    if (!propose_message.empty()) {
        if (propose_success) {
            ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "%s", propose_message.c_str());
        } else {
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "%s", propose_message.c_str());
        }
    }

    // close button
    if (ImGui::Button("Close")) {
        show_window = false;
        propose_success = false;
        propose_message = "";
    }

    ImGui::End();
}

void error_callback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}