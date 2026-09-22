float rotX = 0;
float rotY = 0;
float velX = 0;
float velY = 0;
float stepVel = 0.02;

PGraphics[] textures = new PGraphics[2];
int colorTheme = 0;

void setup() {
  size(900, 700, P3D);
  smooth(8);
  generarTexturas();
}

void draw() {
  background(18, 20, 28);

  ambientLight(70, 70, 85);
  directionalLight(255, 230, 200, 0.5, 1, -0.7);
  pointLight(120, 180, 255, width * 0.2, height * 0.2, 400);
  lightSpecular(255, 255, 255);

  rotX += velX;
  rotY += velY;

  pushMatrix();
  translate(width / 2.0, height / 2.0, 0);

  pushMatrix();
  rotateX(rotX);
  rotateY(rotY);

  pushMatrix();
  noFill();
  stroke(220, 240, 255, 180);
  strokeWeight(1.2);
  specular(255, 255, 255);
  shininess(15.0);
  dibujarToroide(130, 45, 36, 18);
  popMatrix();
  noStroke();
  fill(255);

  pushMatrix();
  dibujarPlanoConTextura(textures[0], 420, 420);
  popMatrix();

  pushMatrix();
  rotateX(HALF_PI);
  dibujarPlanoConTextura(textures[1], 420, 420);
  popMatrix();

  popMatrix();
  popMatrix();

  camara2D();
}

void dibujarPlanoConTextura(PImage tex, float w, float h) {
  float hw = w / 2.0;
  float hh = h / 2.0;

  beginShape(QUADS);
  texture(tex);
  vertex(-hw, -hh, 0, 0, 0);
  vertex( hw, -hh, 0, tex.width, 0);
  vertex( hw,  hh, 0, tex.width, tex.height);
  vertex(-hw,  hh, 0, 0, tex.height);
  endShape();

  beginShape(QUADS);
  texture(tex);
  vertex( hw, -hh, 0, 0, 0);
  vertex(-hw, -hh, 0, tex.width, 0);
  vertex(-hw,  hh, 0, tex.width, tex.height);
  vertex( hw,  hh, 0, 0, tex.height);
  endShape();
}

// generar texturas
void generarTexturas() {
  for (int i = 0; i < 2; i++) {
    textures[i] = createGraphics(400, 400);
    textures[i].beginDraw();
    textures[i].background(15);

    int gridSize = 16;
    float cell = textures[i].width / (float) gridSize;

    for (int gx = 0; gx < gridSize; gx++) {
      for (int gy = 0; gy < gridSize; gy++) {
        boolean checker = (gx + gy) % 2 == 0;
//cambiar los colores
        switch (colorTheme % 3) {
          case 0:
            textures[i].fill(checker ? color(240, 100, 40) : color(40, 180, 120));
            break;
          case 1:
            textures[i].fill(checker ? color(30, 120, 240) : color(240, 220, 60));
            break;
          case 2:
            float dist = dist(gx, gy, gridSize / 2.0, gridSize / 2.0);
            textures[i].fill(((int)dist % 2 == 0) ? color(200, 40, 100) : color(230));
            break;
        }

        textures[i].stroke(10);
        textures[i].strokeWeight(1);
        textures[i].rect(gx * cell, gy * cell, cell, cell);
      }
    }

    textures[i].pushMatrix();
    textures[i].translate(textures[i].width * 0.3, textures[i].height * 0.25);
    textures[i].rotate(-QUARTER_PI);
    textures[i].fill(255);
    textures[i].textSize(18);
    textures[i].textAlign(CENTER, CENTER);
    textures[i].text("TP4 EJ1 Nicolas Patricelli" + (i + 1), 0, 0);
    textures[i].popMatrix();

    textures[i].endDraw();
  }
}

// dibujare toroide
void dibujarToroide(float R, float r, int numR, int numr) {
  for (int i = 0; i < numR; i++) {
    float theta1 = map(i, 0, numR, 0, TWO_PI);
    float theta2 = map(i + 1, 0, numR, 0, TWO_PI);

    beginShape(TRIANGLE_STRIP);
    for (int j = 0; j <= numr; j++) {
      float phi = map(j, 0, numr, 0, TWO_PI);

      float x1 = (R + r * cos(phi)) * cos(theta1);
      float y1 = (R + r * cos(phi)) * sin(theta1);
      float z1 = r * sin(phi);

      float x2 = (R + r * cos(phi)) * cos(theta2);
      float y2 = (R + r * cos(phi)) * sin(theta2);
      float z2 = r * sin(phi);

      vertex(x1, y1, z1);
      vertex(x2, y2, z2);
    }
    endShape();
  }
}

// controles de teclado
void keyPressed() {
  if (key == CODED) {
    if (keyCode == UP)    velX -= stepVel;
    if (keyCode == DOWN)  velX += stepVel;
    if (keyCode == LEFT)  velY -= stepVel;
    if (keyCode == RIGHT) velY += stepVel;
  } else {
    if (key == 'd' || key == 'D') {
      velX = 0;
      velY = 0;
    }
    if (key == 'r' || key == 'R') {
      rotX = 0;
      rotY = 0;
      velX = 0.01;
      velY = 0.015;
      colorTheme++;
      generarTexturas();
    }
  }
}

void camara2D() {
  hint(DISABLE_DEPTH_TEST);
  fill(240);
  textSize(13);
  text("Flechas: Rotar X/Y | D: Detener | R: Reiniciar + Cambiar Patrones", 20, 30);
  hint(ENABLE_DEPTH_TEST);
}
