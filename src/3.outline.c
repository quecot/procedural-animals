#include <math.h>
#include <raylib.h>
#include <sys/_types/_size_t.h>

//------------------------------------------------------------------------------------------
// Types and Structures Definition
//------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void) {
  // Initialization (variables and assets)
  //--------------------------------------------------------------------------------------
  const int SCREEN_WIDTH       = 800;
  const int SCREEN_HEIGHT      = 450;
  const Color BACKGROUND_COLOR = {255, 255, 255, 255};

  bool paused = false;

  const float LINE_WIDTH = 3.0;

  // Head
  const float HEAD_RADIUS   = 37;
  Vector2 head_position     = {-150.0, -150.0};
  const float HEAD_VELOCITY = 5.0;
  bool head_stopped         = false;
  const int HEAD_DOT_COUNT  = 12;
  Vector2 head_dots[HEAD_DOT_COUNT];
  Vector2 left_eye_position  = {0, 0};
  Vector2 right_eye_position = {0, 0};

  // Body parts
  const float BODY_DISTANCE = 30;
  const int BODY_PARTS      = 13;
  Vector2 body_positions[BODY_PARTS];
  Vector2 left_body_dots[BODY_PARTS];
  Vector2 right_body_dots[BODY_PARTS];
  float body_radii[] = {42, 43.5, 42.5, 41.5, 38.5, 32, 30, 25.5, 18, 17, 16, 9.5, 15};

  const int TAIL_DOT_COUNT = 8;
  Vector2 tail_dots[TAIL_DOT_COUNT];

  for (int i = 0; i < BODY_PARTS; i++) {
    body_positions[i] = (Vector2){-150.0, -150.0};
  }

  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Procedural Animals");

  SetTargetFPS(60);

  //--------------------------------------------------------------------------------------

  // Main game loop
  while (!WindowShouldClose()) // Detect window close button or ESC key
  {
    // Update
    //----------------------------------------------------------------------------------
    if (IsKeyPressed(KEY_SPACE)) {
      paused = !paused;
    }

    if (!paused) {
      float mouse_x = GetMouseX();
      float mouse_y = GetMouseY();

      float distance = sqrt(pow(mouse_x - head_position.x, 2) + pow(mouse_y - head_position.y, 2));

      if (!head_stopped && distance > HEAD_VELOCITY) {
        // Advance head towards the mouse
        float angle      = atan2(mouse_y - head_position.y, mouse_x - head_position.x);
        head_position.x += cos(angle) * HEAD_VELOCITY;
        head_position.y += sin(angle) * HEAD_VELOCITY;

        for (size_t i = 0; i < HEAD_DOT_COUNT; i++) {
          head_dots[i] = (Vector2){
              head_position.x + cos(angle + PI / HEAD_DOT_COUNT * i - PI / 2) * HEAD_RADIUS,
              head_position.y + sin(angle + PI / HEAD_DOT_COUNT * i - PI / 2) * HEAD_RADIUS};
        }

        left_eye_position  = (Vector2){head_position.x + cos(angle + PI / 4) * (HEAD_RADIUS - 12),
                                       head_position.y + sin(angle + PI / 4) * (HEAD_RADIUS - 12)};
        right_eye_position = (Vector2){head_position.x + cos(angle - PI / 4) * (HEAD_RADIUS - 12),
                                       head_position.y + sin(angle - PI / 4) * (HEAD_RADIUS - 12)};
      } else if (!head_stopped) {
        head_stopped = true;
      }

      if (head_stopped && distance > (HEAD_VELOCITY + HEAD_RADIUS)) {
        head_stopped = false;
      }

      // Update body parts applying a max distance constraint between them
      for (size_t i = 0; i < BODY_PARTS; i++) {
        Vector2 target_position = (i == 0) ? head_position : body_positions[i - 1];

        distance = sqrt(pow(target_position.x - body_positions[i].x, 2) +
                        pow(target_position.y - body_positions[i].y, 2));

        if (distance > BODY_DISTANCE) {
          float angle          = atan2(target_position.y - body_positions[i].y,
                                       target_position.x - body_positions[i].x);
          body_positions[i].x += cos(angle) * (distance - BODY_DISTANCE);
          body_positions[i].y += sin(angle) * (distance - BODY_DISTANCE);

          left_body_dots[i]  = (Vector2){body_positions[i].x + cos(angle + PI / 2) * body_radii[i],
                                         body_positions[i].y + sin(angle + PI / 2) * body_radii[i]};
          right_body_dots[i] = (Vector2){body_positions[i].x + cos(angle - PI / 2) * body_radii[i],
                                         body_positions[i].y + sin(angle - PI / 2) * body_radii[i]};

          if (i == BODY_PARTS - 1) {
            for (size_t j = 0; j < TAIL_DOT_COUNT; j++) {
              float angle_offset = PI / 2 + (PI / (TAIL_DOT_COUNT - 1)) * j;
              tail_dots[j]       = (Vector2){
                  body_positions[i].x + cos(angle - angle_offset) * body_radii[i],
                  body_positions[i].y + sin(angle - angle_offset) * body_radii[i],
              };
            }
          }
        }
      }
    }

    //----------------------------------------------------------------------------------

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();

    ClearBackground(BACKGROUND_COLOR);

    // Draw body parts
    for (size_t i = 0; i < BODY_PARTS; i++) {
      // Join the dots between body parts
      if (i > 0) {
        DrawLineEx(left_body_dots[i - 1], left_body_dots[i], LINE_WIDTH, BLACK);
        DrawLineEx(right_body_dots[i - 1], right_body_dots[i], LINE_WIDTH, BLACK);
      } else {
        DrawLineEx(head_dots[HEAD_DOT_COUNT - 1], left_body_dots[0], LINE_WIDTH, BLACK);
        DrawLineEx(head_dots[0], right_body_dots[0], LINE_WIDTH, BLACK);
      }
    }

    // Draw head
    for (size_t i = 0; i < HEAD_DOT_COUNT - 1; i++) {
      DrawLineEx(head_dots[i], head_dots[i + 1], LINE_WIDTH, BLACK);
    }

    DrawCircleV(left_eye_position, 5, BLACK);
    DrawCircleV(right_eye_position, 5, BLACK);

    // Draw tail
    for (size_t i = 0; i < TAIL_DOT_COUNT - 1; i++) {
      DrawLineEx(tail_dots[i], tail_dots[i + 1], LINE_WIDTH, BLACK);
    }

    // Draw UI
    char *pause_text =
        paused ? "Press SPACE to unpause the movement" : "Press SPACE to pause the movement";
    DrawText(pause_text, SCREEN_WIDTH / 2 - (paused ? 143 : 135), SCREEN_HEIGHT - 20, 15, DARKGRAY);

    EndDrawing();
    //----------------------------------------------------------------------------------
  }

  // De-Initialization: unload all loaded data (textures, fonts, audio)
  //--------------------------------------------------------------------------------------
  CloseWindow();
  //--------------------------------------------------------------------------------------

  return 0;
}
