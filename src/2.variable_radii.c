#include <math.h>
#include <raylib.h>
#include "rlgl.h"
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
  const int screen_width       = 800;
  const int screen_height      = 450;
  const Color background_color = {255, 255, 255, 255};

  bool paused = false;

  // Head
  const float head_radius   = 68 / 2;
  Vector2 head_position     = {-150.0, -150.0};
  const float head_velocity = 5.0;
  bool head_stopped         = false;

  // Body parts
  const float body_distance = 40;
  const int body_parts      = 13;
  float body_radii[13]      = {84 / 2, 87 / 2, 85 / 2, 83 / 2, 77 / 2, 64 / 2, 60 / 2,
                               51 / 2, 38 / 2, 34 / 2, 32 / 2, 19 / 2, 15 / 2};
  Vector2 body_positions[body_parts];

  for (int i = 0; i < body_parts; i++) {
    body_positions[i] = (Vector2){-150.0, -150.0};
  }

  InitWindow(screen_width, screen_height, "Procedural Animals");

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

      if (!head_stopped && distance > head_velocity) {
        // Advance head towards the mouse
        float angle      = atan2(mouse_y - head_position.y, mouse_x - head_position.x);
        head_position.x += cos(angle) * head_velocity;
        head_position.y += sin(angle) * head_velocity;
      } else if (!head_stopped) {
        head_stopped = true;
      }

      if (head_stopped && distance > (head_velocity + head_radius)) {
        head_stopped = false;
      }

      // Update body parts applying a max distance constraint between them
      for (size_t i = 0; i < body_parts; i++) {
        Vector2 target_position = (i == 0) ? head_position : body_positions[i - 1];

        distance = sqrt(pow(target_position.x - body_positions[i].x, 2) +
                        pow(target_position.y - body_positions[i].y, 2));

        if (distance > body_distance) {
          float angle          = atan2(target_position.y - body_positions[i].y,
                                       target_position.x - body_positions[i].x);
          body_positions[i].x += cos(angle) * (distance - body_distance);
          body_positions[i].y += sin(angle) * (distance - body_distance);
        }
      }
    }

    //----------------------------------------------------------------------------------

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();

    ClearBackground(background_color);

    // Draw body parts
    for (size_t i = 0; i < body_parts; i++) {
      DrawLineEx((i == 0) ? head_position : body_positions[i - 1], body_positions[i], body_radii[i],
                 LIGHTGRAY);
      DrawCircleV(body_positions[i], body_radii[i], LIGHTGRAY);
    }
    for (size_t i = 0; i < body_parts; i++) {
      DrawCircleV(body_positions[i], body_radii[i] - 6, WHITE);
    }

    // Draw head
    DrawCircleV(head_position, head_radius, DARKGRAY);
    DrawCircleV(head_position, head_radius - 6, WHITE);

    // Draw UI
    char *pause_text =
        paused ? "Press SPACE to unpause the movement" : "Press SPACE to pause the movement";
    DrawText(pause_text, screen_width / 2 - (paused ? 143 : 135), screen_height - 20, 15, DARKGRAY);

    EndDrawing();
    //----------------------------------------------------------------------------------
  }

  // De-Initialization: unload all loaded data (textures, fonts, audio)
  //--------------------------------------------------------------------------------------
  CloseWindow();
  //--------------------------------------------------------------------------------------

  return 0;
}
