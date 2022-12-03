#include "gui.hpp"

static void glfw_error_callback(int error, const char* description) {
  fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}
std::string ReplaceAll(std::string str, const std::string& from,
                       const std::string& to) {
  size_t start_pos = 0;
  while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
    str.replace(start_pos, from.length(), to);
    start_pos +=
        to.length();  // Handles case where 'to' is a substring of 'from'
  }
  return str;
}
int main(int, char**) {
  glfwSetErrorCallback(glfw_error_callback);
  if (!glfwInit()) return 1;

#if defined(IMGUI_IMPL_OPENGL_ES2)
  const char* glsl_version = "#version 100";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
  const char* glsl_version = "#version 150";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#else
  const char* glsl_version = "#version 130";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
#endif
  GLFWwindow* window = glfwCreateWindow(1280, 720, "Calculator", NULL, NULL);
  if (window == NULL) return 1;
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImPlot::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  (void)io;

  ImGui::StyleColorsLight();

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init(glsl_version);

  bool showcredit = false, showdeposit = false, capitalization = false,
       graph = false;
  ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
  char expression[256];
  double sum = 0, taxes = 0, withdraw = 0, replenishments = 0, interest = 0;
  int term = 0;
  static int type_payment = 0;
  double cr_sum = 0, cr_interest = 0;
  int cr_term = 0;
  static int cr_type_payment = 0;
  double x_data[GRAPH_LEN] = {0};
  double xs[GRAPH_LEN];
  double y_data[GRAPH_LEN] = {0};
  long double calc_res = 0;
  Bank credit_res = {}, deposit_res = {};
  ImGuiKey key = ImGuiKey_Equal, lshift = ImGuiKey_LeftShift;

  ImPlotStyle& plot_style = ImPlot::GetStyle();
  plot_style.PlotDefaultSize = ImVec2(300, 400);

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    {
      ImGui::Begin("Smart calculator");
      ImGui::Text("Enter the evaluation");
      ImGuiInputTextFlags input_text_flags =
          ImGuiInputTextFlags_EscapeClearsAll |
          ImGuiInputTextFlags_CharsNoBlank;
      ImGui::InputText("##", expression, 256, input_text_flags);
      ImGui::SameLine();
      if ((ImGui::Button("=") || ImGui::IsKeyPressed(key)) &&
          !ImGui::IsKeyDown(lshift)) {
        std::string s(expression);
        graph = true;
        for (int i = 0, j = -GRAPH_LEN / 2; i < GRAPH_LEN; i++, j++) {
          xs[i] = j * 0.01f;
          std::string tmp =
              ReplaceAll(s, std::string("x"), std::to_string(xs[i]));
          const char* a = tmp.c_str();
          y_data[i] = xs[i];
          x_data[i] = solver((char*)a);
          if (i == 0 && !tmp.compare(s)) calc_res = solver((char*)a);
        }
      }
      ImGui::Text("Your result = %.7Lf", calc_res);
      ImGui::Checkbox("Credit calculator", &showcredit);
      ImGui::Checkbox("Deposit calculator", &showdeposit);
      ImGui::End();
    }
    if (graph) {
      ImGui::Begin("Graph", &graph);
      if (ImPlot::BeginPlot("##")) {
        static float constraints[4] = {-100, 100, 10, 100};
        ImPlot::SetupAxesLimits(-10, 10, -10, 10);
        ImPlot::SetupAxisLimitsConstraints(ImAxis_X1, constraints[0],
                                           constraints[1]);
        ImPlot::SetupAxisZoomConstraints(ImAxis_X1, constraints[2],
                                         constraints[3]);
        ImPlot::SetupAxisLimitsConstraints(ImAxis_Y1, constraints[0],
                                           constraints[1]);
        ImPlot::SetupAxisZoomConstraints(ImAxis_Y1, constraints[2],
                                         constraints[3]);
        ImPlot::PlotLine("##", y_data, x_data, GRAPH_LEN);
        ImPlot::EndPlot();
      }
      ImGui::End();
    }
    if (showcredit) {
      ImGui::Begin("Credit Calculator", &showcredit);
      ImGui::InputDouble("Sum", &cr_sum);
      ImGui::InputInt("Term", &cr_term);
      ImGui::InputDouble("Interest", &cr_interest);
      ImGui::RadioButton("Annuity", &cr_type_payment, 0);
      ImGui::SameLine();
      ImGui::RadioButton("Differentiated", &cr_type_payment, 1);
      if (ImGui::Button("Evaluate"))
        credit_res =
            credit(cr_sum, cr_term, cr_interest, (bool)cr_type_payment);
      ImGui::Text("First month payment: %.7Lf", credit_res.res1[0]);
      ImGui::Text("Last month payment: %.7Lf", credit_res.res1[1]);
      ImGui::Text("Overpayment on credit: %.7Lf", credit_res.res2);
      ImGui::Text("Total payment: %.7Lf", credit_res.res3);
      ImGui::End();
    }
    if (showdeposit) {
      ImGui::Begin("Deposit Calculator", &showdeposit);
      ImGui::InputDouble("Sum", &sum);
      ImGui::InputInt("Term", &term);
      ImGui::InputDouble("Interest", &interest);
      ImGui::InputDouble("Taxes", &taxes);
      ImGui::RadioButton("Monthly", &type_payment, 0);
      ImGui::SameLine();
      ImGui::RadioButton("Yearly", &type_payment, 1);
      ImGui::Checkbox("Capitalization", &capitalization);
      ImGui::InputDouble("Replenishments", &replenishments);
      ImGui::InputDouble("Withdraw", &withdraw);
      if (ImGui::Button("Evaluate"))
        deposit_res = deposit(sum, term, interest, taxes, type_payment,
                              capitalization, replenishments, withdraw);
      ImGui::Text("Accrued interest: %.7Lf", deposit_res.res1[0]);
      ImGui::Text("Taxes: %.7Lf", deposit_res.res2);
      ImGui::Text("The amount on the deposit: %.7Lf", deposit_res.res3);
      ImGui::End();
    }
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w,
                 clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
  }

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImPlot::DestroyContext();
  ImGui::DestroyContext();

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}