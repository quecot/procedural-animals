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
  const Color FILL_COLOR       = {103, 212, 219, 255};

  bool paused = false;

  float mouse_x;
  float mouse_y;

  const float LINE_WIDTH = 3.0;

  // Head
  const float HEAD_RADIUS   = 37;
  Vector2 head_position     = {-150.0, -150.0};
  const float HEAD_VELOCITY = 5.0;
  bool head_stopped         = false;
  const int HEAD_DOT_COUNT  = 18;
  Vector2 head_dots[HEAD_DOT_COUNT];
  Vector2 left_eye_position  = {0, 0};
  Vector2 right_eye_position = {0, 0};

  // Body parts
  const float BODY_DISTANCE = 2;
  const int BODY_PARTS      = 300;
  Vector2 body_positions[BODY_PARTS];
  Vector2 left_body_dots[BODY_PARTS];
  Vector2 right_body_dots[BODY_PARTS];
  float body_radii[BODY_PARTS];

  for (int i = 0; i < BODY_PARTS; i++) {
    body_positions[i] = (Vector2){-150.0, -150.0};
    body_radii[i]     = HEAD_RADIUS - (HEAD_RADIUS - 5) * (i / (float)BODY_PARTS);
  }

  const int TAIL_DOT_COUNT = 8;
  Vector2 tail_dots[TAIL_DOT_COUNT];

  const float MAX_ANGLE_DIFFERENCE = PI / 8;

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
      mouse_x = GetMouseX();
      mouse_y = GetMouseY();

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

            left_body_dots[i] =
                (Vector2){body_positions[i].x + cos(angle + PI / 2) * body_radii[i],
                          body_positions[i].y + sin(angle + PI / 2) * body_radii[i]};
            right_body_dots[i] =
                (Vector2){body_positions[i].x + cos(angle - PI / 2) * body_radii[i],
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

          // Angular constraint
          Vector2 prev_segment    = (i == 0)   ? (Vector2){mouse_x, mouse_y}
                                    : (i == 1) ? head_position
                                               : body_positions[i - 2];
          Vector2 current_segment = (i == 0) ? head_position : body_positions[i - 1];
          Vector2 next_segment    = body_positions[i];

          float angle1 =
              atan2(current_segment.y - prev_segment.y, current_segment.x - prev_segment.x);
          float angle2 =
              atan2(next_segment.y - current_segment.y, next_segment.x - current_segment.x);

          float angle_diff = angle2 - angle1;

          if (angle_diff > PI)
            angle_diff -= 2 * PI;
          if (angle_diff < -PI)
            angle_diff += 2 * PI;

          if (fabs(angle_diff) > MAX_ANGLE_DIFFERENCE) {
            float correction_angle =
                (angle_diff > 0) ? angle1 + MAX_ANGLE_DIFFERENCE : angle1 - MAX_ANGLE_DIFFERENCE;
            float correction_distance = sqrt(pow(next_segment.x - current_segment.x, 2) +
                                             pow(next_segment.y - current_segment.y, 2));

            body_positions[i].x = current_segment.x + cos(correction_angle) * correction_distance;
            body_positions[i].y = current_segment.y + sin(correction_angle) * correction_distance;
          }
        }
      } else if (!head_stopped) {
        head_stopped = true;
      }

      if (head_stopped && distance > (HEAD_VELOCITY + HEAD_RADIUS)) {
        head_stopped = false;
      }
    }

    //----------------------------------------------------------------------------------

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();

    ClearBackground(BACKGROUND_COLOR);

    // Draw body parts with fill and stroke
    for (int i = BODY_PARTS - 1; i >= 0; i--) {
      // Ensure we are not accessing out of bounds for body parts
      if (i > 0) {
        // Draw filled triangles for body parts
        Vector2 triangle_points[3] = {left_body_dots[i - 1], right_body_dots[i - 1],
                                      left_body_dots[i]};
        DrawTriangle(triangle_points[0], triangle_points[1], triangle_points[2], FILL_COLOR);

        triangle_points[0] = right_body_dots[i - 1];
        triangle_points[1] = right_body_dots[i];
        triangle_points[2] = left_body_dots[i];
        DrawTriangle(triangle_points[0], triangle_points[1], triangle_points[2], FILL_COLOR);
      } else {
        // Draw filled triangles joining head to the first body part
        Vector2 first_triangle_points[3]  = {head_dots[HEAD_DOT_COUNT - 1], right_body_dots[0],
                                             left_body_dots[0]};
        Vector2 second_triangle_points[3] = {head_dots[HEAD_DOT_COUNT - 1], head_dots[0],
                                             right_body_dots[0]};

        DrawTriangle(first_triangle_points[0], first_triangle_points[1], first_triangle_points[2],
                     FILL_COLOR);
        DrawTriangle(second_triangle_points[0], second_triangle_points[1],
                     second_triangle_points[2], FILL_COLOR);
      }

      if (i == BODY_PARTS - 1) {
        // Draw the tail
        DrawCircleV(body_positions[i], body_radii[i], FILL_COLOR);
        for (size_t i = 0; i < TAIL_DOT_COUNT - 1; i++) {
          DrawLineEx(tail_dots[i], tail_dots[i + 1], LINE_WIDTH, BLACK);
        }
      }

      // Draw the body stroke
      if (i > 0) {
        DrawLineEx(left_body_dots[i - 1], left_body_dots[i], LINE_WIDTH, BLACK);
        DrawLineEx(right_body_dots[i - 1], right_body_dots[i], LINE_WIDTH, BLACK);
      } else {
        // Draw the head with fill and stroke
        float angle = atan2(head_dots[0].y - head_dots[HEAD_DOT_COUNT - 1].y,
                            head_dots[0].x - head_dots[HEAD_DOT_COUNT - 1].x);
        DrawCircleSector(head_position, HEAD_RADIUS, angle * RAD2DEG, (angle + PI) * RAD2DEG, 90,
                         FILL_COLOR);

        // Draw the head outline
        for (size_t i = 0; i < HEAD_DOT_COUNT - 1; i++) {
          DrawLineEx(head_dots[i], head_dots[i + 1], LINE_WIDTH, BLACK);
        }

        // Draw the stroke joining the head to the first body part
        DrawLineEx(head_dots[HEAD_DOT_COUNT - 1], left_body_dots[0], LINE_WIDTH, BLACK);
        DrawLineEx(head_dots[0], right_body_dots[0], LINE_WIDTH, BLACK);
      }
    }

    // Draw eyes
    DrawCircleV(left_eye_position, 5, BLACK);
    DrawCircleV(right_eye_position, 5, BLACK);

    // Draw the mouse
    DrawCircle(mouse_x, mouse_y, 5, RED);

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
