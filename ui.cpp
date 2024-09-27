#include "ui.h"
#include <GLFW/glfw3.h>
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "src/distributed_ledger/ledger.h"
#include "src/version_1/proof_of_authority/poa.h"
#include <vector>

#include <string>
#include <cstdio>

int startUI() {
    // BACKEND
    Ledger ledger = Ledger::create_mock_ledger();

    // Proof of authority manager
    PoA poa;

    std::vector<Inventory> inventories = ledger.get_inventories();
    if (inventories.size() < 4) {
        fprintf(stderr, "Error: Not enough inventories to designate validators.\n");
        return 1;
    }

    // Designate the first three inventories as validators
    poa.add_validator(inventories[0]);
    poa.add_validator(inventories[1]);
    poa.add_validator(inventories[2]);
    poa.list_validators();
    printf("/n");

    // init GLFW
    glfwSetErrorCallback(error_callback);
    if (!glfwInit()) {
        fprintf(stderr, "Error: Failed to initialize GLFW.\n");
        return 1;
    }

    // GL+GLSL versions for Mac
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // create GLFW window
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Q1 - Version", nullptr, nullptr);
    if (window == nullptr) {
        fprintf(stderr, "Error: Failed to create GLFW window.\n");
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // setup context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();

    // setup renderer
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // UI variables
    bool main_menu = true;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    int selected_inventory = -1; // No inventory selected initially

    // main loop
    while (!glfwWindowShouldClose(window)) {
        // start frame
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // menu Window
        ImGui::Begin("Menu");
        for (size_t i = 0; i < inventories.size(); ++i) {
            std::string button_label = "Show Inventory " + std::to_string(i + 1);
            if (ImGui::Button(button_label.c_str())) {
                selected_inventory = static_cast<int>(i);
            }
        }
        ImGui::End();

        // display inventory window
        if (selected_inventory >= 0 && selected_inventory < static_cast<int>(inventories.size())) {
            show_inventory_as_table(inventories[selected_inventory], selected_inventory);
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

    std::string inventory_name = inventory.get_inventory_name();
    ImGui::TextUnformatted(inventory_name.c_str());
    ImGui::Separator();

    // create table
    if (ImGui::BeginTable("InventoryTable", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
        ImGui::TableSetupColumn("Item ID");
        ImGui::TableSetupColumn("Item Price");
        ImGui::TableSetupColumn("Item Qty");
        ImGui::TableSetupColumn("Location");

        // row headers
        ImGui::TableHeadersRow();

        // populate table rows
        for (const auto& block : inventory.get_blocks()) {
            const Transaction& tx = block.get_transaction();

            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::TextUnformatted(tx.get_item_id().c_str());

            ImGui::TableSetColumnIndex(1);
            ImGui::Text("%d", tx.get_item_price());

            ImGui::TableSetColumnIndex(2);
            ImGui::Text("%d", tx.get_item_qty());

            ImGui::TableSetColumnIndex(3);

            ImGui::Text("%d", static_cast<int>(tx.get_location()));
        }

        ImGui::EndTable();
    }

    // close button
    if (ImGui::Button("Close")) {
        selected_inventory = -1;
    }

    ImGui::End();
}

void error_callback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}
