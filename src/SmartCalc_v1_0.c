#include "SmartCalc_v1_0.h"

static bool EqualBracketCount(char *str) {
  int openingBracketCount = 0;
  int closingBracketCount = 0;

  while (*str != '\0') {
    if (*str == '(') {
      openingBracketCount++;
    }
    if (*str == ')') {
      closingBracketCount++;
    }

    str++;
  }

  return (openingBracketCount == closingBracketCount) ? true : false;
}

static bool CorrectDigitInString(char *str) {
  while (*str != '\0') {
    if (*str == '.') {
      if (!(*(str - 1) <= '9' && *(str - 1) >= '0') ||
          !(*(str + 1) <= '9' && *(str + 1) >= '0')) {
        return false;
      }
    } else if (*str <= '9' && *str >= '0') {
      if (*(str + 1) >= 'a' && *(str + 1) <= 'z') {
        return false;
      } else if (*(str - 1) >= 'a' && *(str - 1) <= 'z') {
        return false;
      }
    }

    str++;
  }

  return true;
}

static bool IsActionOrDot(char action, bool withDot) {
  char const actionsArray[7] = {'+', '-', '*', '/', '%', '^', '.'};

  for (int i = 0; i < ((withDot) ? 7 : 6); i++) {
    if (action == actionsArray[i]) {
      return true;
    }
  }

  return false;
}

static bool NotDoubleActionInString(char *str) {
  int index = 0;

  while (*str != '\0') {
    if (IsActionOrDot(*str, true)) {
      if (IsActionOrDot(*(str - 1), true) || IsActionOrDot(*(str + 1), true)) {
        return false;
      }
    }

    str++;
    index++;
  }

  return true;
}

static bool NotDoubleXInString(char *str) {
  while (*str != '\0') {
    if (*str == 'x') {
      if (*(str - 1) == 'x' || *(str + 1) == 'x') {
        return false;
      }
    }

    str++;
  }

  return true;
}

static bool isCorrectX(char *str) {
  while (*str != '\0') {
    if (*str == 'x') {
      if ((*(str - 1) <= '9' && *(str - 1) >= '0') ||
          (*(str + 1) <= '9' && *(str + 1) >= '0')) {
        return false;
      } else if (*(str - 1) == ')' || *(str + 1) == '(') {
        return false;
      } else if (*(str - 1) == '.' || *(str + 1) == '.') {
        return false;
      }
    }

    str++;
  }

  return true;
}

static bool isCorrectDigitWithBracket(char *str) {
  while (*str != '\0') {
    if (*str == ')') {
      if (*(str + 1) <= '9' && *(str + 1) >= '0') {
        return false;
      }
    } else if (*str == '(') {
      if (*(str - 1) <= '9' && *(str - 1) >= '0') {
        return false;
      }
    }

    str++;
  }

  return true;
}

static bool isCorrectActionsWithSymbol(char *str) {
  while (*str != '\0') {
    if (IsActionOrDot(*(str + 1), false)) {
      if (IsActionOrDot(*(str + 2), false) || *(str + 2) == ')') {
        return false;
      }
    }

    str++;
  }

  return true;
}

static bool isCorrectFunc(char *func) {
  bool isFunc = false;

  if (strcmp(func, "sqrt(") == 0) {
    isFunc = true;
  } else if (strcmp(func, "acos(") == 0) {
    isFunc = true;
  } else if (strcmp(func, "atan(") == 0) {
    isFunc = true;
  } else if (strcmp(func, "asin(") == 0) {
    isFunc = true;
  } else if (strcmp(func, "cos(") == 0) {
    isFunc = true;
  } else if (strcmp(func, "tan(") == 0) {
    isFunc = true;
  } else if (strcmp(func, "sin(") == 0) {
    isFunc = true;
  } else if (strcmp(func, "log(") == 0) {
    isFunc = true;
  } else if (strcmp(func, "ln(") == 0) {
    isFunc = true;
  }

  return isFunc;
}

static bool isFuncForCalc(char *str) {
  while (*str != '\0') {
    if (*str >= 'a' && *str <= 't') {
      char func[256] = {0};

      if (*str == 'a' ||
          (*str == 's' && *(str + 1) == 'q')) {  // acos, atan, asin, sqrt
        strncat(func, str, 5);
        if (!isCorrectFunc(func)) {
          return false;
        }

        str += 5;
      } else if (*str == 'l' && *(str + 1) == 'n') {
        strncat(func, str, 2);
        if (!isCorrectFunc(func)) {
          return false;
        }

        str += 2;
      } else {  // cos, tan, sin, log
        strncat(func, str, 4);
        if (!isCorrectFunc(func)) {
          return false;
        }

        str += 4;
      }
    } else {
      str++;
    }
  }

  return true;
}

bool isCorrectTrigString(char *str) {
  if (!isFuncForCalc(str)) {
    return false;
  }
  if (!EqualBracketCount(str)) {
    return false;
  }
  if (!CorrectDigitInString(str)) {
    return false;
  }
  if (!NotDoubleActionInString(str)) {
    return false;
  }
  if (!NotDoubleXInString(str)) {
    return false;
  }
  if (!isCorrectX(str)) {
    return false;
  }
  if (!isCorrectDigitWithBracket(str)) {
    return false;
  }
  if (!isCorrectActionsWithSymbol(str)) {
    return false;
  }

  return true;
}

bool IsCorrectString(char *str) {
  if (!EqualBracketCount(str)) {
    return false;
  }
  if (!CorrectDigitInString(str)) {
    return false;
  }
  if (!NotDoubleActionInString(str)) {
    return false;
  }
  if (!isFuncForCalc(str)) {
    return false;
  }
  if (!isCorrectDigitWithBracket(str)) {
    return false;
  }
  if (!isCorrectActionsWithSymbol(str)) {
    return false;
  }

  return true;
}

static void CreateErrorWindow(App *application) {
  application->errorWindow = gtk_application_window_new(application->app);

  GtkWidget *errorLabel = gtk_label_new("Incorrect input");
  GtkWidget *errorMainBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 20);
  GtkWidget *errorTopBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  GtkWidget *errorMidBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

  if (application->dialog != NULL) {
    gtk_window_close(GTK_WINDOW(application->dialog));
  }

  gtk_window_set_title(GTK_WINDOW(application->errorWindow), "Error");
  gtk_window_set_default_size(GTK_WINDOW(application->errorWindow), 150, 50);
  gtk_window_set_resizable(GTK_WINDOW(application->errorWindow), false);

  gtk_window_set_child(GTK_WINDOW(application->errorWindow), errorMainBox);

  gtk_box_append(GTK_BOX(errorMainBox), errorTopBox);
  gtk_box_append(GTK_BOX(errorMainBox), errorMidBox);
  gtk_box_append(GTK_BOX(errorMidBox), errorLabel);

  gtk_widget_show(errorLabel);
  gtk_widget_show(application->errorWindow);
}

static void DestroyGraphWindow(GtkWidget *button, App *application) {
  application->graph = NULL;
}

static double GetStepsCount(int xRange) {
  double max = 10000;
  double step = xRange;

  if (xRange == 10) {
    step = 1;
  } else {
    while (step > 10) {
      step /= 10;
    }
  }

  double steps_col = xRange < 100.0 ? 100.0 * step : max * step;

  return steps_col;
}

static void DrawCoordLines(cairo_t *cr, double x1, double y1, double x2, double y2, const gchar *text) {
  cairo_move_to(cr, x1, y1);

  if (text == NULL) {
    cairo_line_to(cr, x2, y2);
  } else {
    cairo_show_text(cr, text);
  }
}

static double ApplyDrawSettings(cairo_t *cr, double xLength, double xStPos, int width, int height) {
  double centerW = width / 2;
  double centerH = height / 2;

  cairo_set_line_width(cr, 0.1);
  cairo_set_font_size(cr, 10);

  DrawCoordLines(cr, 0, centerH, width, centerH, NULL);
  DrawCoordLines(cr, centerW, 0, centerW, height, NULL);
  DrawCoordLines(cr, centerW + 5, centerH - 5, 0, 0, "0");
  DrawCoordLines(cr, width - 15, centerH - 20, 0, 0, "x");
  DrawCoordLines(cr, centerW + 10, 20, 0, 0, "y");

  double widthLine = xLength * 0.002 + 20 * 0.002;
  double xScale = width / ((xLength + fabs(xStPos) + 10) * 2);
  double yScale = (height / ((xLength + fabs(xStPos) + 10) * 2)) * -1;

  cairo_translate(cr, centerW, centerH);
  cairo_set_line_width(cr, widthLine);
  cairo_scale(cr, xScale, yScale);

  return GetStepsCount(fabs(xLength + fabs(xStPos)));
}

static void DrawGraph(GtkDrawingArea *drawing_area, cairo_t *cr, int width, int height, App *application) {
  char expression[256] = {0};
  char xPos[256] = {0};
  char xNeg[256] = {0};
  double xPosDouble = 0;
  double xNegDouble = 0;
  double yDouble = 0;
  double xDouble = 0;
  int counter = 0;

  gtk_window_close(GTK_WINDOW(application->dialog));

  strcpy(expression, "(");
  strcat(expression, gtk_entry_buffer_get_text(application->bufferEntry));
  strcat(expression, ")");

  strcpy(xPos, gtk_entry_buffer_get_text(application->bufferEntryDialogPosX));
  strcpy(xNeg, gtk_entry_buffer_get_text(application->bufferEntryDialogNegX));

  sscanf(xPos, "%lf", &xPosDouble);
  sscanf(xNeg, "%lf", &xNegDouble);

  xDouble = xNegDouble;

  double stepsCount = ApplyDrawSettings(cr, xPosDouble, xNegDouble, width, height);
  double step = (xPosDouble) / (stepsCount);

  while (stepsCount != 0) {
    char expressionOnExit[256] = {0};
    int j = 0;

    for (int i = 0; expression[i] != '\0'; i++) {
      if (expression[i] == 'x') {
        if (xNeg[0] == '-' && expression[i - 1] != '(') {
          while (j < i) {
            expressionOnExit[j] = expression[j];
            j++;
          }

          char x[256] = {0};
          sprintf(x, "%lf", xDouble);

          expressionOnExit[j++] = '(';

          for (int k = 0; k < strlen(x); k++) {
            expressionOnExit[j++] = x[k];
          }

          expressionOnExit[j++] = ')';
        } else {
          while (j < i) {
            expressionOnExit[j] = expression[j];
            j++;
          }

          char x[256] = {0};
          sprintf(x, "%lf", xDouble);

          for (int k = 0; k < strlen(x); k++) {
            expressionOnExit[j++] = x[k];
          }
        }
      } else {
        expressionOnExit[j] = expression[i];
        j++;
      }
    }
    char y[256] = {0};
    char *yPointer;

    yPointer = Parser(expressionOnExit);

    for (int i = 0; *(yPointer + i) != '\0'; i++) {
      y[i] = *(yPointer + i);
    }

    sscanf(y, "%lf", &yDouble);

    if (counter < 1) {
      cairo_move_to(cr, xDouble, yDouble);
    }
    cairo_line_to(cr, xDouble, yDouble);

    xDouble += step;
    counter++;
    stepsCount--;

    free(yPointer);
  }
  cairo_stroke(cr);
}

static void CreateGraphWindow(GtkWidget *button, App *application) {
  GtkWidget *drawArea = gtk_drawing_area_new();
  GtkWidget *boxDrawArea = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

  application->graph = gtk_application_window_new(application->app);

  g_signal_connect(application->graph, "destroy", G_CALLBACK(DestroyGraphWindow), &application);
  gtk_window_set_title(GTK_WINDOW(application->graph), "Graph");
  gtk_window_set_default_size(GTK_WINDOW(application->graph), 300, 300);
  gtk_window_set_resizable(GTK_WINDOW(application->graph), false);

  gtk_window_set_child(GTK_WINDOW(application->graph), boxDrawArea);
  gtk_box_append(GTK_BOX(boxDrawArea), drawArea);
  gtk_widget_set_hexpand(boxDrawArea, true);
  gtk_widget_set_hexpand(drawArea, true);

  gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(drawArea), (GtkDrawingAreaDrawFunc)DrawGraph, application, NULL);

  gtk_widget_show(application->graph);
}

static void DestroyDialogWindow(GtkWidget *dialog, App *application) {
  application->bufferEntryDialogPosX = NULL;
  application->bufferEntryDialogNegX = NULL;
  application->entryDialogPosX = NULL;
  application->entryDialogNegX = NULL;
  application->labelPosX = NULL;
  application->labelNegX = NULL;
  application->dialogGrid = NULL;
  application->boxMainDialog = NULL;
  application->dialog = NULL;
}

static void DestroyApp(GtkWidget *app, App *application) {
  if (application->graph == NULL) {
  } else {
    gtk_window_close(GTK_WINDOW(application->graph));
  }

  if (application->errorWindow == NULL) {
  } else {
    gtk_window_close(GTK_WINDOW(application->errorWindow));
  }

  if (application->trigs == NULL) {
  } else {
    gtk_window_close(GTK_WINDOW(application->trigs));
  }

  if (application->actions == NULL) {
  } else {
    gtk_window_close(GTK_WINDOW(application->actions));
  }
}

static void SubstractionLengthX(GtkWidget *button, App *application) {
  char posX[256] = {0};
  char result[256] = {0};
  int posXInt = 0;

  strcpy(posX, gtk_entry_buffer_get_text(application->bufferEntryDialogPosX));
  sscanf(posX, "%i", &posXInt);

  if (posXInt - 5 > 0) {
    posXInt -= 5;

    sprintf(result, "%i", posXInt);

    char *onExitEntryText = result;

    gtk_entry_buffer_set_text(application->bufferEntryDialogPosX, onExitEntryText, strlen(onExitEntryText));
  }
}

static void AdditionLengthX(GtkWidget *button, App *application) {
  char posX[256] = {0};
  char result[256] = {0};
  char *onExitEntryText;
  int posXInt = 0;

  strcpy(posX, gtk_entry_buffer_get_text(application->bufferEntryDialogPosX));
  sscanf(posX, "%i", &posXInt);

  posXInt += 5;

  sprintf(result, "%i", posXInt);
  onExitEntryText = result;

  gtk_entry_buffer_set_text(application->bufferEntryDialogPosX, onExitEntryText, strlen(onExitEntryText));
}

static void SubstractionStPosX(GtkWidget *button, App *application) {
  char negX[256] = {0};
  char result[256] = {0};
  char *onExitEntryText;
  int negXInt = 0;

  strcpy(negX, gtk_entry_buffer_get_text(application->bufferEntryDialogNegX));
  sscanf(negX, "%i", &negXInt);

  negXInt -= 5;

  sprintf(result, "%i", negXInt);
  onExitEntryText = result;

  gtk_entry_buffer_set_text(application->bufferEntryDialogNegX, onExitEntryText, strlen(onExitEntryText));
}

static void AdditionStPosX(GtkWidget *button, App *application) {
  char negX[256] = {0};
  char posX[256] = {0};
  char result[256] = {0};
  char *onExitEntryText;
  int negXInt = 0;
  int posXInt = 0;

  strcpy(negX, gtk_entry_buffer_get_text(application->bufferEntryDialogNegX));
  sscanf(negX, "%i", &negXInt);

  strcpy(posX, gtk_entry_buffer_get_text(application->bufferEntryDialogPosX));
  sscanf(posX, "%i", &posXInt);

  negXInt += 5;

  sprintf(result, "%i", negXInt);
  onExitEntryText = result;

  gtk_entry_buffer_set_text(application->bufferEntryDialogNegX, onExitEntryText, strlen(onExitEntryText));
}

static void AddElementsToTheDialogGrid(App *application) {
  gtk_grid_attach(GTK_GRID(application->dialogGrid), application->labelPosX, 0,
                  0, 1, 1);
  gtk_grid_attach(GTK_GRID(application->dialogGrid), application->labelNegX, 0,
                  1, 1, 1);

  gtk_grid_attach(GTK_GRID(application->dialogGrid), application->entryDialogPosX, 1, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(application->dialogGrid), application->entryDialogNegX, 1, 1, 1, 1);
}

static void CreateDialogButtons(App *application) {
  GtkWidget *dialogButtonDialogLengthXA = gtk_button_new_with_label("+");
  GtkWidget *dialogButtonDialogStPosXA = gtk_button_new_with_label("+");
  GtkWidget *dialogButtonDialogLengthXS = gtk_button_new_with_label("-");
  GtkWidget *dialogButtonDialogStPosXS = gtk_button_new_with_label("-");
  GtkWidget *paintGraph = gtk_button_new_with_label("Paint");

  g_signal_connect(GTK_BUTTON(dialogButtonDialogLengthXA), "clicked", G_CALLBACK(AdditionLengthX), application);
  g_signal_connect(GTK_BUTTON(dialogButtonDialogLengthXS), "clicked", G_CALLBACK(SubstractionLengthX), application);
  g_signal_connect(GTK_BUTTON(dialogButtonDialogStPosXA), "clicked", G_CALLBACK(AdditionStPosX), application);
  g_signal_connect(GTK_BUTTON(dialogButtonDialogStPosXS), "clicked", G_CALLBACK(SubstractionStPosX), application);

  g_signal_connect(GTK_BUTTON(paintGraph), "clicked", G_CALLBACK(CreateGraphWindow), application);

  gtk_grid_attach(GTK_GRID(application->dialogGrid), dialogButtonDialogLengthXA, 3, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(application->dialogGrid), dialogButtonDialogLengthXS, 4, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(application->dialogGrid), dialogButtonDialogStPosXA, 3, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(application->dialogGrid), dialogButtonDialogStPosXS, 4, 1, 1, 1);

  gtk_box_append(GTK_BOX(application->boxMainDialog), paintGraph);
}

static void CreateDialogGrid(App *application) {
  application->dialogGrid = gtk_grid_new();

  gtk_box_append(GTK_BOX(application->boxMainDialog), application->dialogGrid);
  gtk_widget_set_hexpand(application->dialogGrid, true);
  gtk_grid_set_row_spacing(GTK_GRID(application->dialogGrid), 10);
  gtk_grid_set_column_spacing(GTK_GRID(application->dialogGrid), 10);

  AddElementsToTheDialogGrid(application);
}

static void CreateDialogLabels(App *application) {
  application->labelPosX = gtk_label_new(" Length x ->");
  application->labelNegX = gtk_label_new(" St pos x ->");
}

static void CreateDialogEntries(App *application) {
  application->bufferEntryDialogPosX = gtk_entry_buffer_new("5", 1);
  application->bufferEntryDialogNegX = gtk_entry_buffer_new("0", 1);

  application->entryDialogPosX = gtk_entry_new_with_buffer(application->bufferEntryDialogPosX);
  application->entryDialogNegX = gtk_entry_new_with_buffer(application->bufferEntryDialogNegX);

  gtk_widget_set_can_focus(application->entryDialogPosX, false);
  gtk_widget_set_can_focus(application->entryDialogNegX, false);

  gtk_entry_set_alignment(GTK_ENTRY(application->entryDialogPosX), 0.5);
  gtk_entry_set_alignment(GTK_ENTRY(application->entryDialogNegX), 0.5);

  gtk_entry_set_max_length(GTK_ENTRY(application->entryDialogPosX), 255);
  gtk_entry_set_max_length(GTK_ENTRY(application->entryDialogNegX), 255);
}

static void CreateDialogElements(App *application) {
  application->boxMainDialog = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

  gtk_box_set_spacing(GTK_BOX(application->boxMainDialog), 10);
  gtk_window_set_child(GTK_WINDOW(application->dialog), application->boxMainDialog);

  CreateDialogEntries(application);
  CreateDialogLabels(application);
  CreateDialogGrid(application);
  CreateDialogButtons(application);
}

static void CreateDialogWindow(GtkWidget *button, App *application) {
  if (application->dialog != NULL) {
    gtk_window_close(GTK_WINDOW(application->dialog));
  }

  char expression[256] = {0};

  strcpy(expression, "(");
  strcat(expression, gtk_entry_buffer_get_text(application->bufferEntry));

  if (strlen(expression) < 2) {
    memset(expression, 0, 256);
    return;
  } else {
    strcat(expression, ")");
  }

  if (isCorrectTrigString(expression)) {
    application->dialog = gtk_application_window_new(application->app);

    g_signal_connect(application->dialog, "destroy", G_CALLBACK(DestroyDialogWindow), &application);
    gtk_window_set_title(GTK_WINDOW(application->dialog), "Definition area");
    gtk_window_set_default_size(GTK_WINDOW(application->dialog), 240, 120);
    gtk_window_set_resizable(GTK_WINDOW(application->dialog), false);

    CreateDialogElements(application);

    gtk_widget_show(application->dialog);
  } else {
    CreateErrorWindow(application);
  }
}

static void DestroyTrigPannel(GtkWidget *button, App *application) {
  application->trigs = NULL;
}

static void DestroyActionPannel(GtkWidget *button, App *application) {
  application->actions = NULL;
}

static void PrintToEntryWithOpenBracket(GtkWidget *button, App *application) {
  char entryText[256] = {0};
  char buttonText[5] = {0};

  strcpy(entryText, gtk_entry_buffer_get_text(application->bufferEntry));
  strcpy(buttonText, gtk_button_get_label(GTK_BUTTON(button)));
  strcat(entryText, buttonText);
  strcat(entryText, "(");

  gtk_entry_buffer_set_text(GTK_ENTRY_BUFFER(application->bufferEntry), entryText, strlen(entryText));
}

static void CreateTrigsWindow(GtkWidget *button, App *application) {
  if (application->trigs == NULL) {
    GtkWidget *trigMainBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 20);
    GtkWidget *trigGrid = gtk_grid_new();

    application->trigs = gtk_application_window_new(application->app);

    g_signal_connect(application->trigs, "destroy", G_CALLBACK(DestroyTrigPannel), application);

    gtk_window_set_title(GTK_WINDOW(application->trigs), "Trigonometry buttons");
    gtk_window_set_default_size(GTK_WINDOW(application->trigs), 100, 170);
    gtk_window_set_resizable(GTK_WINDOW(application->trigs), false);
    gtk_window_set_child(GTK_WINDOW(application->trigs), trigMainBox);

    gtk_box_append(GTK_BOX(trigMainBox), trigGrid);

    gtk_grid_set_row_spacing(GTK_GRID(trigGrid), 5);
    gtk_grid_set_column_spacing(GTK_GRID(trigGrid), 5);

    for (int i = 0; i < 2; i++) {
      for (int j = 0; j < 5; j++) {
        GtkWidget *buttonLabel = gtk_button_new_with_label(trigLabels[i][j]);

        if (i == 1 && j == 4) {
          g_signal_connect(GTK_BUTTON(buttonLabel), "clicked", G_CALLBACK(PrintToEntry), application);
        } else {
          g_signal_connect(GTK_BUTTON(buttonLabel), "clicked", G_CALLBACK(PrintToEntryWithOpenBracket), application);
        }

        gtk_grid_attach(GTK_GRID(trigGrid), buttonLabel, i, j, 1, 1);
      }
    }

    gtk_widget_show(application->trigs);
  }
}

static void CreateActionsWindow(GtkWidget *button, App *application) {
  if (application->actions == NULL) {
    GtkWidget *actionMainBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 20);
    GtkWidget *actionGrid = gtk_grid_new();

    application->actions = gtk_application_window_new(application->app);

    g_signal_connect(application->actions, "destroy", G_CALLBACK(DestroyActionPannel), application);

    gtk_window_set_title(GTK_WINDOW(application->actions), "Action buttons");
    gtk_window_set_default_size(GTK_WINDOW(application->actions), 100, 150);
    gtk_window_set_resizable(GTK_WINDOW(application->actions), false);
    gtk_window_set_child(GTK_WINDOW(application->actions), actionMainBox);

    gtk_box_append(GTK_BOX(actionMainBox), actionGrid);

    gtk_grid_set_row_spacing(GTK_GRID(actionGrid), 5);
    gtk_grid_set_column_spacing(GTK_GRID(actionGrid), 5);

    for (int i = 0; i < 2; i++) {
      for (int j = 0; j < 4; j++) {
        GtkWidget *buttonLabel = gtk_button_new_with_label(actionLabels[i][j]);

        g_signal_connect(GTK_BUTTON(buttonLabel), "clicked", G_CALLBACK(PrintToEntry), application);

        gtk_grid_attach(GTK_GRID(actionGrid), buttonLabel, i, j, 1, 1);
      }
    }

    gtk_widget_show(application->actions);
  }
}

static void AddButtonsToTheGrid(App *application) {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 4; j++) {
      GtkWidget *button = gtk_button_new_with_label(labels[i][j]);

      if (strcmp(labels[i][j], "=") == 0) {
        g_signal_connect(GTK_BUTTON(button), "clicked", G_CALLBACK(Calc), application);
      } else {
        g_signal_connect(GTK_BUTTON(button), "clicked", G_CALLBACK(PrintToEntry), application);
      }

      gtk_grid_attach(GTK_GRID(application->grid), button, i, j, 1, 1);
    }
  }
}

static void CreateGrid(App *application) {
  GtkWidget *boxLeft = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

  application->grid = gtk_grid_new();

  gtk_box_append(GTK_BOX(application->boxGrid), boxLeft);
  gtk_box_append(GTK_BOX(application->boxGrid), application->grid);

  gtk_widget_set_hexpand(application->grid, true);

  gtk_grid_set_row_spacing(GTK_GRID(application->grid), 5);
  gtk_grid_set_column_spacing(GTK_GRID(application->grid), 5);

  AddButtonsToTheGrid(application);
}

static void ClearSymbolInEntry(GtkWidget *button, App *application) {
  char entryText[256] = {0};
  char *entryTextPointer;
  char entryTextOnExit[256] = {0};
  char errorMessage[6] = {0};

  strcpy(entryText, gtk_entry_buffer_get_text(application->bufferEntry));

  entryTextPointer = entryText;

  while (*entryTextPointer != '\0') {
    if (*entryTextPointer == 'e') {
      strncat(errorMessage, entryTextPointer, 5);
      break;
    }

    entryTextPointer++;
  }

  if (strcmp(errorMessage, "error") == 0) {
    memset(entryText, 0, 256);
  }

  strncat(entryTextOnExit, entryText, strlen(entryText) - 1);

  gtk_entry_buffer_set_text(GTK_ENTRY_BUFFER(application->bufferEntry), entryTextOnExit, strlen(entryTextOnExit));
}

static void CreateEntry(App *application) {
  GtkWidget *boxLeft = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  GtkWidget *boxRight = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  GtkWidget *clearButton = gtk_button_new_with_label("C");

  application->bufferEntry = gtk_entry_buffer_new("0", 1);
  application->entry = gtk_entry_new_with_buffer(application->bufferEntry);

  g_signal_connect(clearButton, "clicked", G_CALLBACK(ClearSymbolInEntry), application);

  gtk_box_append(GTK_BOX(application->boxEntry), boxLeft);
  gtk_box_append(GTK_BOX(application->boxEntry), application->entry);
  gtk_box_append(GTK_BOX(application->boxEntry), clearButton);
  gtk_box_append(GTK_BOX(application->boxEntry), boxRight);
  gtk_widget_set_can_focus(application->entry, false);

  gtk_entry_set_alignment(GTK_ENTRY(application->entry), 0.5);
  gtk_widget_set_hexpand(application->entry, true);
  gtk_entry_set_max_length(GTK_ENTRY(application->entry), 255);
}

static void CreateOpenButtons(App *application) {
  GtkWidget *boxLeft = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  GtkWidget *boxRight = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

  GtkWidget *openButtonsGrid = gtk_grid_new();
  GtkWidget *buttonForActions = gtk_button_new_with_label("Act");
  GtkWidget *buttonForTrigs = gtk_button_new_with_label("Func");
  GtkWidget *buttonForGraph = gtk_button_new_with_label("Grp");

  gtk_box_append(GTK_BOX(application->boxOpenButtons), boxLeft);
  gtk_box_append(GTK_BOX(application->boxOpenButtons), openButtonsGrid);
  gtk_box_append(GTK_BOX(application->boxOpenButtons), boxRight);

  gtk_grid_set_row_spacing(GTK_GRID(openButtonsGrid), 5);
  gtk_grid_set_column_spacing(GTK_GRID(openButtonsGrid), 5);

  g_signal_connect(buttonForActions, "clicked", G_CALLBACK(CreateActionsWindow), application);
  g_signal_connect(buttonForTrigs, "clicked", G_CALLBACK(CreateTrigsWindow), application);
  g_signal_connect(buttonForGraph, "clicked", G_CALLBACK(CreateDialogWindow), application);

  gtk_grid_attach(GTK_GRID(openButtonsGrid), buttonForActions, 0, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(openButtonsGrid), buttonForTrigs, 1, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(openButtonsGrid), buttonForGraph, 2, 0, 1, 1);
}

static void CreateElements(App *application) {
  GtkWidget *boxUp = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  GtkWidget *boxDown = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

  application->boxMain = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  application->boxOpenButtons = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
  application->boxEntry = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
  application->boxGrid = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 13);

  gtk_box_set_spacing(GTK_BOX(application->boxMain), 5);
  gtk_window_set_child(GTK_WINDOW(application->window), application->boxMain);

  gtk_box_append(GTK_BOX(application->boxMain), boxUp);
  gtk_box_append(GTK_BOX(application->boxMain), application->boxOpenButtons);
  gtk_box_append(GTK_BOX(application->boxMain), application->boxEntry);
  gtk_box_append(GTK_BOX(application->boxMain), application->boxGrid);
  gtk_box_append(GTK_BOX(application->boxMain), boxDown);

  CreateOpenButtons(application);
  CreateEntry(application);
  CreateGrid(application);
}

static void CreateApp(GtkApplication *app, App *application) {
  application->window = gtk_application_window_new(application->app);

  g_signal_connect(application->window, "destroy", G_CALLBACK(DestroyApp), application);
  gtk_window_set_title(GTK_WINDOW(application->window), "Calculator");
  gtk_window_set_default_size(GTK_WINDOW(application->window), 160, 170);
  gtk_window_set_resizable(GTK_WINDOW(application->window), false);

  CreateElements(application);

  gtk_widget_show(application->window);
}

static int RunApp(int argc, char **argv) {
  static App application;
  int status;

  application.app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);

  g_signal_connect(application.app, "activate", G_CALLBACK(CreateApp), &application);

  status = g_application_run(G_APPLICATION(application.app), argc, argv);

  g_object_unref(application.app);

  return status;
}

int main(int argc, char **argv) { return RunApp(argc, argv); }

static void PrintToEntry(GtkWidget *button, App *application) {
  char entryText[256] = {0};
  char buttonText[5] = {0};

  strcpy(entryText, gtk_entry_buffer_get_text(application->bufferEntry));
  strcpy(buttonText, gtk_button_get_label(GTK_BUTTON(button)));
  strcat(entryText, buttonText);

  gtk_entry_buffer_set_text(GTK_ENTRY_BUFFER(application->bufferEntry), entryText, strlen(entryText));
}

static void Calc(GtkWidget *button, App *application) {
  char onStartEntryText[256] = {0};
  char *onExitEntryText = NULL;
  int onStartEntryTextLength;
  bool expressionHaveX = false;
  bool isParse = false;

  strcpy(onStartEntryText, "(");
  strcat(onStartEntryText, gtk_entry_buffer_get_text(application->bufferEntry));

  if (strlen(onStartEntryText) < 2) {
    memset(onStartEntryText, 0, 256);
    return;
  } else {
    strcat(onStartEntryText, ")");
  }

  onStartEntryTextLength = strlen(onStartEntryText);

  for (int i = 0; i < onStartEntryTextLength; i++) {
    if (onStartEntryText[i] == 'x') {
      expressionHaveX = true;
      onExitEntryText = "input x";
    }
  }

  if (!expressionHaveX) {
    if (IsCorrectString(onStartEntryText)) {
      onExitEntryText = Parser(onStartEntryText);
      isParse = true;
    } else {
      onExitEntryText = "error";
    }
  }

  gtk_entry_buffer_set_text(GTK_ENTRY_BUFFER(application->bufferEntry), onExitEntryText, strlen(onExitEntryText));

  if (isParse) { free(onExitEntryText); }
}
