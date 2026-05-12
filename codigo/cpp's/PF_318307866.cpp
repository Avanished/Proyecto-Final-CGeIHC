#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>
#include <stdlib.h>

#ifdef _WIN32
#include <windows.h>
#endif

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Skybox.h"
#include "Model.h"

#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"

const float toRadians = 3.14159265f / 180.0f;

// ======================================================
// SISTEMA: Window, shaders, camara, texturas, skybox
// ======================================================
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;
Camera camera;

Texture pisoTexture;
Skybox skyboxDia;
Skybox skyboxNoche;

// ======================================================
// MODELOS - ESCENARIO
// ======================================================
Model Pared_M;          // Muralla/pared del escenario
Model PaloFarola_M;     // Poste de la farola
Model FocoDer_M;        // Foco derecho de la farola
Model FocoIzq_M;        // Foco izquierdo de la farola
Model Banca_M;          // Banca del parque
Model BloquePasto_M;    // Bloque de pasto decorativo
Model BasePlanetas_M;   // Base del reloj de planetas
Model RelojBase_M;      // Cuerpo principal del reloj
Model RelojEngrane1_M;  // Engrane 1 del reloj (orbita Y-)
Model RelojEngrane2_M;  // Engrane 2 del reloj (orbita Y+)
Model RelojEngrane3_M;  // Engrane 3 del reloj (orbita Y-)
Model RelojEngrane4_M;  // Engrane 4 del reloj (orbita Y+)
Model RelojGalRedE_M;   // Galaxia/red del reloj (orbita Y-)
Model RelojPlanetas_M;  // Planetas del reloj (orbita Y-)

// ======================================================
// MODELOS - AVATARES
// ======================================================

// --- Avatar BioShock: Big Daddy ---
Model BigDaddyCuerpo_M;   // Torso principal del Big Daddy
Model BigDaddyTaladro_M;  // Taladro del brazo derecho
Model BigDaddyBrazoIzq_M; // Brazo izquierdo
Model BigDaddyBrazoDer_M; // Brazo derecho
Model BigDaddyPiernaIzq_M;// Pierna izquierda
Model BigDaddyPiernaDer_M;// Pierna derecha

// --- Avatar Harry Potter: Hermione ---
Model Hermione_HP_M;   // Cuerpo completo de Hermione
Model BrazoDer_HP_M;   // Brazo derecho de Hermione
Model BrazoIzq_HP_M;   // Brazo izquierdo de Hermione
Model PiernaDer_HP_M;  // Pierna derecha de Hermione
Model PiernaIzq_HP_M;  // Pierna izquierda de Hermione

// --- Avatar Crash Bandicoot: Crash ---
Model Crash_Cuerpo_M;    // Cuerpo principal de Crash
Model Crash_BrazoDer_M;  // Brazo derecho de Crash
Model Crash_BrazoIzq_M;  // Brazo izquierdo de Crash
Model Crash_PiernaDer_M; // Pierna derecha de Crash
Model Crash_PiernaIzq_M; // Pierna izquierda de Crash

// ======================================================
// MODELOS - OBJETOS
// ======================================================

// --- Objetos BioShock ---
Model BolsaDer_M;   // Tapa derecha del medkit
Model BolsaIzq_M;   // Tapa izquierda del medkit
Model BolsaMid_M;   // Cuerpo central del medkit
Model Plasmido_M;   // Frasco de plasmido (flota y gira)

// --- Objetos Harry Potter ---
Model Carro_HP_M;        // Carro volador de Harry Potter
Model LlantaFD_HP_M;     // Llanta frontal derecha del carro HP
Model LlantaFI_HP_M;     // Llanta frontal izquierda del carro HP
Model LlantaAD_HP_M;     // Llanta trasera derecha del carro HP
Model LlantaAI_HP_M;     // Llanta trasera izquierda del carro HP
Model Bolsa_HP_M;        // Bolsa de Hermione
Model Giratiempo_HP_M;   // Giratiempo (time-turner)
Model LibroHechizos_HP_M;// Libro de hechizos
Model Snitch_HP_M;       // Snitch dorada (animacion de vuelo)
Model Copa_HP_M;         // Copa de los tres magos

// --- Objetos Harry Potter: Expreso de Hogwarts ---
Model CabinaExpreso_M;         // Cabina principal del Expreso de Hogwarts
Model VagonExpreso_M;          // Vagon del Expreso de Hogwarts
Model BielaIzqExpreso_M;       // Biela izquierda del Expreso
Model BielaMotrizIzqExpreso_M; // Biela motriz izquierda del Expreso
Model EjeDelanExpreso_M;       // Eje delantero del Expreso
Model EjeTrasExpreso_M;        // Eje trasero del Expreso
Model EjeVagonExpreso_M;       // Eje del vagon del Expreso
Model PistonIzqExpreso_M;      // Piston izquierdo del Expreso
Model LlantaSteamCar3_M;       // Llanta del Expreso (steamcar3)
Model SteamCar3_M;             // Modelo completo Expreso/bicicleta Harry

// --- Objetos Crash Bandicoot ---
Model Crash_GoKart_M;      // Go-kart de Crash
Model LlantaFD_Crash_M;    // Llanta frontal derecha del kart
Model LlantaFI_Crash_M;    // Llanta frontal izquierda del kart
Model LlantaAD_Crash_M;    // Llanta trasera derecha del kart
Model LlantaAI_Crash_M;    // Llanta trasera izquierda del kart
Model SteamCar_Crash_M;    // Steam car de Crash (decorativo)

// --- Objetos Crash Bandicoot: props del universo ---
Model ArbolWumpa_M;            // Arbol de fruta Wumpa
Model BaseCilindro_M;          // Base cilindrica decorativa
Model BolaRoca_M;              // Bola de roca
Model CajaAku_M;               // Caja con cara de Aku Aku
Model CajaCaraCrash_M;         // Caja con cara de Crash
Model CajaCheckPoint_M;        // Caja de checkpoint
Model CajaFlechaArriba_M;      // Caja con flecha hacia arriba
Model CajaMetalExclamation_M;  // Caja metalica con signo !
Model CajaNitro_M;             // Caja de nitro
Model CajaNitroExclamation_M;  // Caja nitro con signo !
Model CajaNormalCrash_M;       // Caja normal de Crash
Model CajaQuestion_M;          // Caja con signo ?
Model CajaTNT_M;               // Caja TNT
Model CocoOnGoKart_M;          // Coco Bandicoot en su kart
Model CortexOnGoKart_M;        // Dr. Cortex en su kart
Model CrystalCrash_M;          // Cristal del universo Crash
Model CrystalHielo_M;          // Cristal de hielo
Model FrutaWumpa_M;            // Fruta Wumpa
Model JoyasCrash_M;            // Joyas del universo Crash
Model MascaraAku_M;            // Mascara de Aku Aku
Model MascaraUka_M;            // Mascara de Uka Uka
Model Pista_M;                 // Pista de carreras
Model SteamCar1_M;             // Steam Car 1 (Crash & Grunt)
Model SteamCar2_M;             // Steam Car 2 (Chief Coco)
Model Totem1Crash_M;           // Totem decorativo 1
Model Totem2Crash_M;           // Totem decorativo 2
Model RuedaSteamCar2_M;        // Rueda del Steam Car 2

// ======================================================
// MATERIALES
// ======================================================
Material Material_brillante; // Specular alto: metales, vidrio
Material Material_opaco;     // Specular bajo: madera, tela, pasto

// ======================================================
// TIEMPO / FPS
// ======================================================
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// ======================================================
// LUCES
// ======================================================
DirectionalLight mainLight;
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight  spotLights[MAX_SPOT_LIGHTS];

// ======================================================
// SHADERS
// ======================================================
static const char* vShader = "shaders/shader_light.vert";
static const char* fShader = "shaders/shader_light.frag";

// ======================================================
// CAPTURA DE WAYPOINTS (C = toggle, P = guardar, L = limpiar)
// ======================================================
bool modoCaptura = false;
int  capturaContador = 0;
std::vector<glm::vec3> waypointsCapturados;

bool teclaCAnterior = false;
bool teclaPAnterior = false;
bool teclaLAnterior = false;

// ======================================================
// HELPER: aplica Translate * RotX * RotY * RotZ * Scale
// ======================================================
glm::mat4 ApplyTRS(glm::mat4 base, glm::vec3 t, glm::vec3 r, glm::vec3 s)
{
    base = glm::translate(base, t);
    base = glm::rotate(base, r.x * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
    base = glm::rotate(base, r.y * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
    base = glm::rotate(base, r.z * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
    base = glm::scale(base, s);
    return base;
}

glm::vec3 SafeNormalize(const glm::vec3& v)
{
    float len = glm::length(v);
    if (len <= 0.0001f) return glm::vec3(0.0f, 0.0f, 0.0f);
    return v / len;
}

void UpdatePathFollower(
    glm::vec3& pos,
    float& rotY,
    int& currentIndex,
    const std::vector<glm::vec3>& path,
    float speed,
    float dt,
    float threshold = 0.60f)
{
    if (path.empty()) return;

    glm::vec3 target = path[currentIndex];
    glm::vec3 delta = target - pos;
    float dist = glm::length(delta);

    if (dist < threshold) {
        currentIndex = (currentIndex + 1) % path.size();
        target = path[currentIndex];
        delta = target - pos;
        dist = glm::length(delta);
    }

    if (dist > 0.0001f) {
        glm::vec3 dir = delta / dist;
        float step = speed * dt;
        if (step > dist) step = dist;
        pos += dir * step;

        rotY = atan2(dir.x, dir.z) / toRadians;
    }
}
// ======================================================
// HELPER: mueve una posicion hacia el origen en X y Z
// ======================================================
glm::vec3 MoveTowardsOriginXZ(glm::vec3 p, float step)
{
    if (p.x > 0.0f)      p.x -= step;
    else if (p.x < 0.0f) p.x += step;
    if (p.z > 0.0f)      p.z -= step;
    else if (p.z < 0.0f) p.z += step;
    return p;
}

// ======================================================
// GEOMETRIA BASE (piso como mesh)
// ======================================================
void CreateObjects()
{
    unsigned int floorIndices[] = { 0, 2, 1, 1, 2, 3 };

    GLfloat floorVertices[] = {
        -10.0f, 0.0f, -10.0f,   0.0f,  0.0f,   0.0f, 1.0f, 0.0f,
         10.0f, 0.0f, -10.0f,  10.0f,  0.0f,   0.0f, 1.0f, 0.0f,
        -10.0f, 0.0f,  10.0f,   0.0f, 10.0f,   0.0f, 1.0f, 0.0f,
         10.0f, 0.0f,  10.0f,  10.0f, 10.0f,   0.0f, 1.0f, 0.0f
    };

    Mesh* floor = new Mesh();
    floor->CreateMesh(floorVertices, floorIndices, 32, 6);
    meshList.push_back(floor);
}

// ======================================================
// SHADERS
// ======================================================
void CreateShaders()
{
    Shader* shader1 = new Shader();
    shader1->CreateFromFiles(vShader, fShader);
    shaderList.push_back(*shader1);
}

// ======================================================
// MAIN
// ======================================================
int main()
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    mainWindow = Window(1366, 768);
    mainWindow.Initialise();
    CreateObjects();
    CreateShaders();

    camera = Camera(
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        -60.0f, 0.0f, 20.0f, 0.3f
    );

    // ------------------------------------------------
    // TEXTURAS Y SKYBOX
    // ------------------------------------------------
    pisoTexture = Texture("Textures/piso.tga");
    pisoTexture.LoadTextureA();

    // Skybox dia
    std::vector<std::string> skyboxFacesDia;
    skyboxFacesDia.push_back("Textures/Skybox/_px.jpg");
    skyboxFacesDia.push_back("Textures/Skybox/_nx.jpg");
    skyboxFacesDia.push_back("Textures/Skybox/_ny.jpg");
    skyboxFacesDia.push_back("Textures/Skybox/_py.jpg");
    skyboxFacesDia.push_back("Textures/Skybox/_pz.jpg");
    skyboxFacesDia.push_back("Textures/Skybox/_nz.jpg");
    skyboxDia = Skybox(skyboxFacesDia);

    // Skybox noche
    std::vector<std::string> skyboxFacesNoche;
    skyboxFacesNoche.push_back("Textures/Skybox/_pxnight.jpg");
    skyboxFacesNoche.push_back("Textures/Skybox/_nxnight.jpg");
    skyboxFacesNoche.push_back("Textures/Skybox/_nynight.jpg");
    skyboxFacesNoche.push_back("Textures/Skybox/_pynight.jpg");
    skyboxFacesNoche.push_back("Textures/Skybox/_pznight.jpg");
    skyboxFacesNoche.push_back("Textures/Skybox/_nznight.jpg");
    skyboxNoche = Skybox(skyboxFacesNoche);

    // ================================================
    // CARGA DE MODELOS - ESCENARIO
    // ================================================
    printf("\n================ ESCENARIO ================\n");

    printf("Cargando Pared_M -> Models/pared.obj ...\n");
    Pared_M = Model(); Pared_M.LoadModel("Models/pared.obj");
    printf("OK -> Pared_M\n");

    printf("Cargando PaloFarola_M -> Models/Palofarola.obj ...\n");
    PaloFarola_M = Model(); PaloFarola_M.LoadModel("Models/Palofarola.obj");
    printf("OK -> PaloFarola_M\n");

    printf("Cargando FocoDer_M -> Models/Focoder.obj ...\n");
    FocoDer_M = Model(); FocoDer_M.LoadModel("Models/Focoder.obj");
    printf("OK -> FocoDer_M\n");

    printf("Cargando FocoIzq_M -> Models/Focoizq.obj ...\n");
    FocoIzq_M = Model(); FocoIzq_M.LoadModel("Models/Focoizq.obj");
    printf("OK -> FocoIzq_M\n");

    printf("Cargando Banca_M -> Models/Banca.obj ...\n");
    Banca_M = Model(); Banca_M.LoadModel("Models/Banca.obj");
    printf("OK -> Banca_M\n");

    printf("Cargando BloquePasto_M -> Models/Bloquepasto.obj ...\n");
    BloquePasto_M = Model(); BloquePasto_M.LoadModel("Models/Bloquepasto.obj");
    printf("OK -> BloquePasto_M\n");

    printf("Cargando BasePlanetas_M -> Models/Baseplanetas.obj ...\n");
    BasePlanetas_M = Model(); BasePlanetas_M.LoadModel("Models/Baseplanetas.obj");
    printf("OK -> BasePlanetas_M\n");

    printf("Cargando RelojBase_M -> Models/relojbase.obj ...\n");
    RelojBase_M = Model(); RelojBase_M.LoadModel("Models/relojbase.obj");
    printf("OK -> RelojBase_M\n");

    printf("Cargando RelojEngrane1_M -> Models/relojengrane1.obj ...\n");
    RelojEngrane1_M = Model(); RelojEngrane1_M.LoadModel("Models/relojengrane1.obj");
    printf("OK -> RelojEngrane1_M\n");

    printf("Cargando RelojEngrane2_M -> Models/relojengrane2.obj ...\n");
    RelojEngrane2_M = Model(); RelojEngrane2_M.LoadModel("Models/relojengrane2.obj");
    printf("OK -> RelojEngrane2_M\n");

    printf("Cargando RelojEngrane3_M -> Models/relojengrane3.obj ...\n");
    RelojEngrane3_M = Model(); RelojEngrane3_M.LoadModel("Models/relojengrane3.obj");
    printf("OK -> RelojEngrane3_M\n");

    printf("Cargando RelojEngrane4_M -> Models/relojengrane4.obj ...\n");
    RelojEngrane4_M = Model(); RelojEngrane4_M.LoadModel("Models/relojengrane4.obj");
    printf("OK -> RelojEngrane4_M\n");

    printf("Cargando RelojGalRedE_M -> Models/relojgalrede.obj ...\n");
    RelojGalRedE_M = Model(); RelojGalRedE_M.LoadModel("Models/relojgalrede.obj");
    printf("OK -> RelojGalRedE_M\n");

    printf("Cargando RelojPlanetas_M -> Models/relojplanetas.obj ...\n");
    RelojPlanetas_M = Model(); RelojPlanetas_M.LoadModel("Models/relojplanetas.obj");
    printf("OK -> RelojPlanetas_M\n");

    // ================================================
    // CARGA DE MODELOS - AVATARES
    // ================================================

    // --- Avatar BioShock ---
    printf("\n================ AVATARES - BIOSHOCK ================\n");

    printf("Cargando BigDaddyCuerpo_M -> Models/Cuerpobig.obj ...\n");
    BigDaddyCuerpo_M = Model(); BigDaddyCuerpo_M.LoadModel("Models/Cuerpobig.obj");
    printf("OK -> BigDaddyCuerpo_M\n");

    printf("Cargando BigDaddyTaladro_M -> Models/taladro.obj ...\n");
    BigDaddyTaladro_M = Model(); BigDaddyTaladro_M.LoadModel("Models/taladro.obj");
    printf("OK -> BigDaddyTaladro_M\n");

    printf("Cargando BigDaddyBrazoIzq_M -> Models/brazoizq.obj ...\n");
    BigDaddyBrazoIzq_M = Model(); BigDaddyBrazoIzq_M.LoadModel("Models/brazoizq.obj");
    printf("OK -> BigDaddyBrazoIzq_M\n");

    printf("Cargando BigDaddyBrazoDer_M -> Models/brazoder.obj ...\n");
    BigDaddyBrazoDer_M = Model(); BigDaddyBrazoDer_M.LoadModel("Models/brazoder.obj");
    printf("OK -> BigDaddyBrazoDer_M\n");

    printf("Cargando BigDaddyPiernaIzq_M -> Models/piernaizq.obj ...\n");
    BigDaddyPiernaIzq_M = Model(); BigDaddyPiernaIzq_M.LoadModel("Models/piernaizq.obj");
    printf("OK -> BigDaddyPiernaIzq_M\n");

    printf("Cargando BigDaddyPiernaDer_M -> Models/piernader.obj ...\n");
    BigDaddyPiernaDer_M = Model(); BigDaddyPiernaDer_M.LoadModel("Models/piernader.obj");
    printf("OK -> BigDaddyPiernaDer_M\n");

    // --- Avatar Harry Potter ---
    printf("\n================ AVATARES - HARRY POTTER ================\n");

    printf("Cargando Hermione_HP_M -> Models/HermioneCompleta.obj ...\n");
    Hermione_HP_M = Model(); Hermione_HP_M.LoadModel("Models/HermioneCompleta.obj");
    printf("OK -> Hermione_HP_M\n");

    printf("Cargando BrazoDer_HP_M -> Models/brazoderhermione.obj ...\n");
    BrazoDer_HP_M = Model(); BrazoDer_HP_M.LoadModel("Models/brazoderhermione.obj");
    printf("OK -> BrazoDer_HP_M\n");

    printf("Cargando BrazoIzq_HP_M -> Models/brazoizqhermione.obj ...\n");
    BrazoIzq_HP_M = Model(); BrazoIzq_HP_M.LoadModel("Models/brazoizqhermione.obj");
    printf("OK -> BrazoIzq_HP_M\n");

    printf("Cargando PiernaDer_HP_M -> Models/piernader.obj ...\n");
    PiernaDer_HP_M = Model(); PiernaDer_HP_M.LoadModel("Models/piernader.obj");
    printf("OK -> PiernaDer_HP_M\n");

    printf("Cargando PiernaIzq_HP_M -> Models/piernaizq.obj ...\n");
    PiernaIzq_HP_M = Model(); PiernaIzq_HP_M.LoadModel("Models/piernaizq.obj");
    printf("OK -> PiernaIzq_HP_M\n");

    // --- Avatar Crash Bandicoot ---
    printf("\n================ AVATARES - CRASH ================\n");

    printf("Cargando Crash_Cuerpo_M -> Models/crashCharacterCuerpo.obj ...\n");
    Crash_Cuerpo_M = Model(); Crash_Cuerpo_M.LoadModel("Models/crashCharacterCuerpo.obj");
    printf("OK -> Crash_Cuerpo_M\n");

    printf("Cargando Crash_BrazoDer_M -> Models/crashCharacterBrazoDerecho.obj ...\n");
    Crash_BrazoDer_M = Model(); Crash_BrazoDer_M.LoadModel("Models/crashCharacterBrazoDerecho.obj");
    printf("OK -> Crash_BrazoDer_M\n");

    printf("Cargando Crash_BrazoIzq_M -> Models/crashCharacterBrazoIzquierdo.obj ...\n");
    Crash_BrazoIzq_M = Model(); Crash_BrazoIzq_M.LoadModel("Models/crashCharacterBrazoIzquierdo.obj");
    printf("OK -> Crash_BrazoIzq_M\n");

    printf("Cargando Crash_PiernaDer_M -> Models/crashCharacterPiernaDerecha.obj ...\n");
    Crash_PiernaDer_M = Model(); Crash_PiernaDer_M.LoadModel("Models/crashCharacterPiernaDerecha.obj");
    printf("OK -> Crash_PiernaDer_M\n");

    printf("Cargando Crash_PiernaIzq_M -> Models/crashCharacterPiernaIzquierda.obj ...\n");
    Crash_PiernaIzq_M = Model(); Crash_PiernaIzq_M.LoadModel("Models/crashCharacterPiernaIzquierda.obj");
    printf("OK -> Crash_PiernaIzq_M\n");

    // ================================================
    // CARGA DE MODELOS - OBJETOS
    // ================================================

    // --- Objetos BioShock ---
    printf("\n================ OBJETOS - BIOSHOCK ================\n");

    printf("Cargando BolsaDer_M -> Models/bolsader.obj ...\n");
    BolsaDer_M = Model(); BolsaDer_M.LoadModel("Models/bolsader.obj");
    printf("OK -> BolsaDer_M\n");

    printf("Cargando BolsaIzq_M -> Models/bolsaizq.obj ...\n");
    BolsaIzq_M = Model(); BolsaIzq_M.LoadModel("Models/bolsaizq.obj");
    printf("OK -> BolsaIzq_M\n");

    printf("Cargando BolsaMid_M -> Models/bolsamid.obj ...\n");
    BolsaMid_M = Model(); BolsaMid_M.LoadModel("Models/bolsamid.obj");
    printf("OK -> BolsaMid_M\n");

    printf("Cargando Plasmido_M -> Models/plasmido.obj ...\n");
    Plasmido_M = Model(); Plasmido_M.LoadModel("Models/plasmido.obj");
    printf("OK -> Plasmido_M\n");

    // --- Objetos Harry Potter ---
    printf("\n================ OBJETOS - HARRY POTTER ================\n");

    printf("Cargando Carro_HP_M -> Models/CarroCompleto.obj ...\n");
    Carro_HP_M = Model(); Carro_HP_M.LoadModel("Models/CarroCompleto.obj");
    printf("OK -> Carro_HP_M\n");

    printf("Cargando LlantaFD_HP_M -> Models/llSuperiorDer.obj ...\n");
    LlantaFD_HP_M = Model(); LlantaFD_HP_M.LoadModel("Models/llSuperiorDer.obj");
    printf("OK -> LlantaFD_HP_M\n");

    printf("Cargando LlantaFI_HP_M -> Models/llSuperiorIzq.obj ...\n");
    LlantaFI_HP_M = Model(); LlantaFI_HP_M.LoadModel("Models/llSuperiorIzq.obj");
    printf("OK -> LlantaFI_HP_M\n");

    printf("Cargando LlantaAD_HP_M -> Models/llAtrasDer.obj ...\n");
    LlantaAD_HP_M = Model(); LlantaAD_HP_M.LoadModel("Models/llAtrasDer.obj");
    printf("OK -> LlantaAD_HP_M\n");

    printf("Cargando LlantaAI_HP_M -> Models/llAtrasIzq.obj ...\n");
    LlantaAI_HP_M = Model(); LlantaAI_HP_M.LoadModel("Models/llAtrasIzq.obj");
    printf("OK -> LlantaAI_HP_M\n");

    printf("Cargando Bolsa_HP_M -> Models/bolsa.obj ...\n");
    Bolsa_HP_M = Model(); Bolsa_HP_M.LoadModel("Models/bolsa.obj");
    printf("OK -> Bolsa_HP_M\n");

    printf("Cargando Giratiempo_HP_M -> Models/giratiempo.obj ...\n");
    Giratiempo_HP_M = Model(); Giratiempo_HP_M.LoadModel("Models/giratiempo.obj");
    printf("OK -> Giratiempo_HP_M\n");

    printf("Cargando LibroHechizos_HP_M -> Models/libroHechizos.obj ...\n");
    LibroHechizos_HP_M = Model(); LibroHechizos_HP_M.LoadModel("Models/libroHechizos.obj");
    printf("OK -> LibroHechizos_HP_M\n");

    printf("Cargando Snitch_HP_M -> Models/snitch.obj ...\n");
    Snitch_HP_M = Model(); Snitch_HP_M.LoadModel("Models/snitch.obj");
    printf("OK -> Snitch_HP_M\n");

    printf("Cargando Copa_HP_M -> Models/copa.obj ...\n");
    Copa_HP_M = Model(); Copa_HP_M.LoadModel("Models/copa.obj");
    printf("OK -> Copa_HP_M\n");

    // Expreso de Hogwarts
    printf("Cargando CabinaExpreso_M -> Models/cabinaExpresoHowarts.obj ...\n");
    CabinaExpreso_M = Model(); CabinaExpreso_M.LoadModel("Models/cabinaExpresoHowarts.obj");
    printf("OK -> CabinaExpreso_M\n");

    printf("Cargando VagonExpreso_M -> Models/vagonExpresoHowarts.obj ...\n");
    VagonExpreso_M = Model(); VagonExpreso_M.LoadModel("Models/vagonExpresoHowarts.obj");
    printf("OK -> VagonExpreso_M\n");

    printf("Cargando BielaIzqExpreso_M -> Models/bielaIzquierdaExpresoHowarts.obj ...\n");
    BielaIzqExpreso_M = Model(); BielaIzqExpreso_M.LoadModel("Models/bielaIzquierdaExpresoHowarts.obj");
    printf("OK -> BielaIzqExpreso_M\n");

    printf("Cargando BielaMotrizIzqExpreso_M -> Models/bielaMotrizIzquierdaExpresoHowarts.obj ...\n");
    BielaMotrizIzqExpreso_M = Model(); BielaMotrizIzqExpreso_M.LoadModel("Models/bielaMotrizIzquierdaExpresoHowarts.obj");
    printf("OK -> BielaMotrizIzqExpreso_M\n");

    printf("Cargando EjeDelanExpreso_M -> Models/ejeDelanteroExpresoHowarts.obj ...\n");
    EjeDelanExpreso_M = Model(); EjeDelanExpreso_M.LoadModel("Models/ejeDelanteroExpresoHowarts.obj");
    printf("OK -> EjeDelanExpreso_M\n");

    printf("Cargando EjeTrasExpreso_M -> Models/ejeTraseroExpresoHowarts.obj ...\n");
    EjeTrasExpreso_M = Model(); EjeTrasExpreso_M.LoadModel("Models/ejeTraseroExpresoHowarts.obj");
    printf("OK -> EjeTrasExpreso_M\n");

    printf("Cargando EjeVagonExpreso_M -> Models/ejeVagonExpresoHowarts.obj ...\n");
    EjeVagonExpreso_M = Model(); EjeVagonExpreso_M.LoadModel("Models/ejeVagonExpresoHowarts.obj");
    printf("OK -> EjeVagonExpreso_M\n");

    printf("Cargando PistonIzqExpreso_M -> Models/pistonIzquierdoExpresoHowarts.obj ...\n");
    PistonIzqExpreso_M = Model(); PistonIzqExpreso_M.LoadModel("Models/pistonIzquierdoExpresoHowarts.obj");
    printf("OK -> PistonIzqExpreso_M\n");

    printf("Cargando LlantaSteamCar3_M -> Models/llantaSteamCar3BikeHarry.obj ...\n");
    LlantaSteamCar3_M = Model(); LlantaSteamCar3_M.LoadModel("Models/llantaSteamCar3BikeHarry.obj");
    printf("OK -> LlantaSteamCar3_M\n");

    printf("Cargando SteamCar3_M -> Models/steamCar3BikeHarry.obj ...\n");
    SteamCar3_M = Model(); SteamCar3_M.LoadModel("Models/steamCar3BikeHarry.obj");
    printf("OK -> SteamCar3_M\n");

    // --- Objetos Crash Bandicoot ---
    printf("\n================ OBJETOS - CRASH ================\n");

    printf("Cargando Crash_GoKart_M -> Models/crashOnGoKart.obj ...\n");
    Crash_GoKart_M = Model(); Crash_GoKart_M.LoadModel("Models/crashOnGoKart.obj");
    printf("OK -> Crash_GoKart_M\n");

    printf("Cargando LlantaFD_Crash_M -> Models/ruedaSteamCar1CrashGrunt.obj ...\n");
    LlantaFD_Crash_M = Model(); LlantaFD_Crash_M.LoadModel("Models/ruedaSteamCar1CrashGrunt.obj");
    printf("OK -> LlantaFD_Crash_M\n");

    printf("Cargando LlantaFI_Crash_M -> Models/ruedaSteamCar1CrashGrunt.obj ...\n");
    LlantaFI_Crash_M = Model(); LlantaFI_Crash_M.LoadModel("Models/ruedaSteamCar1CrashGrunt.obj");
    printf("OK -> LlantaFI_Crash_M\n");

    printf("Cargando LlantaAD_Crash_M -> Models/ruedaSteamCar1CrashGrunt.obj ...\n");
    LlantaAD_Crash_M = Model(); LlantaAD_Crash_M.LoadModel("Models/ruedaSteamCar1CrashGrunt.obj");
    printf("OK -> LlantaAD_Crash_M\n");

    printf("Cargando LlantaAI_Crash_M -> Models/ruedaSteamCar1CrashGrunt.obj ...\n");
    LlantaAI_Crash_M = Model(); LlantaAI_Crash_M.LoadModel("Models/ruedaSteamCar1CrashGrunt.obj");
    printf("OK -> LlantaAI_Crash_M\n");

    printf("Cargando SteamCar_Crash_M -> Models/steamCar.obj ...\n");
    SteamCar_Crash_M = Model();
    SteamCar_Crash_M.LoadModel("Models/steamCar.obj");
    printf("OK -> SteamCar_Crash_M\n");

    // Props del universo Crash
    printf("Cargando ArbolWumpa_M -> Models/arbolWumpa.obj ...\n");
    ArbolWumpa_M = Model(); ArbolWumpa_M.LoadModel("Models/arbolWumpa.obj");
    printf("OK -> ArbolWumpa_M\n");

    printf("Cargando BaseCilindro_M -> Models/baseCilindroCrash.obj ...\n");
    BaseCilindro_M = Model(); BaseCilindro_M.LoadModel("Models/baseCilindroCrash.obj");
    printf("OK -> BaseCilindro_M\n");

    printf("Cargando BolaRoca_M -> Models/bolaRocaCrash.obj ...\n");
    BolaRoca_M = Model(); BolaRoca_M.LoadModel("Models/bolaRocaCrash.obj");
    printf("OK -> BolaRoca_M\n");

    printf("Cargando CajaAku_M -> Models/cajaAku.obj ...\n");
    CajaAku_M = Model(); CajaAku_M.LoadModel("Models/cajaAku.obj");
    printf("OK -> CajaAku_M\n");

    printf("Cargando CajaCaraCrash_M -> Models/cajaCaraCrash.obj ...\n");
    CajaCaraCrash_M = Model(); CajaCaraCrash_M.LoadModel("Models/cajaCaraCrash.obj");
    printf("OK -> CajaCaraCrash_M\n");

    printf("Cargando CajaCheckPoint_M -> Models/cajaCheckPoint.obj ...\n");
    CajaCheckPoint_M = Model(); CajaCheckPoint_M.LoadModel("Models/cajaCheckPoint.obj");
    printf("OK -> CajaCheckPoint_M\n");

    printf("Cargando CajaFlechaArriba_M -> Models/cajaFlechaArriba.obj ...\n");
    CajaFlechaArriba_M = Model(); CajaFlechaArriba_M.LoadModel("Models/cajaFlechaArriba.obj");
    printf("OK -> CajaFlechaArriba_M\n");

    printf("Cargando CajaMetalExclamation_M -> Models/cajaMetalExclamation.obj ...\n");
    CajaMetalExclamation_M = Model(); CajaMetalExclamation_M.LoadModel("Models/cajaMetalExclamation.obj");
    printf("OK -> CajaMetalExclamation_M\n");

    printf("Cargando CajaNitro_M -> Models/cajaNitro.obj ...\n");
    CajaNitro_M = Model(); CajaNitro_M.LoadModel("Models/cajaNitro.obj");
    printf("OK -> CajaNitro_M\n");

    printf("Cargando CajaNitroExclamation_M -> Models/cajaNitroExclamation.obj ...\n");
    CajaNitroExclamation_M = Model(); CajaNitroExclamation_M.LoadModel("Models/cajaNitroExclamation.obj");
    printf("OK -> CajaNitroExclamation_M\n");

    printf("Cargando CajaNormalCrash_M -> Models/cajaNormalCrash.obj ...\n");
    CajaNormalCrash_M = Model(); CajaNormalCrash_M.LoadModel("Models/cajaNormalCrash.obj");
    printf("OK -> CajaNormalCrash_M\n");

    printf("Cargando CajaQuestion_M -> Models/cajaQuestion.obj ...\n");
    CajaQuestion_M = Model(); CajaQuestion_M.LoadModel("Models/cajaQuestion.obj");
    printf("OK -> CajaQuestion_M\n");

    printf("Cargando CajaTNT_M -> Models/cajaTNT.obj ...\n");
    CajaTNT_M = Model(); CajaTNT_M.LoadModel("Models/cajaTNT.obj");
    printf("OK -> CajaTNT_M\n");

    printf("Cargando CocoOnGoKart_M -> Models/cocoOnGoKart.obj ...\n");
    CocoOnGoKart_M = Model(); CocoOnGoKart_M.LoadModel("Models/cocoOnGoKart.obj");
    printf("OK -> CocoOnGoKart_M\n");

    printf("Cargando CortexOnGoKart_M -> Models/cortexOnGoKart.obj ...\n");
    CortexOnGoKart_M = Model(); CortexOnGoKart_M.LoadModel("Models/cortexOnGoKart.obj");
    printf("OK -> CortexOnGoKart_M\n");

    printf("Cargando CrystalCrash_M -> Models/crystalCrash.obj ...\n");
    CrystalCrash_M = Model(); CrystalCrash_M.LoadModel("Models/crystalCrash.obj");
    printf("OK -> CrystalCrash_M\n");

    printf("Cargando CrystalHielo_M -> Models/crystalHielo.obj ...\n");
    CrystalHielo_M = Model(); CrystalHielo_M.LoadModel("Models/crystalHielo.obj");
    printf("OK -> CrystalHielo_M\n");

    printf("Cargando FrutaWumpa_M -> Models/frutaWumpa.obj ...\n");
    FrutaWumpa_M = Model(); FrutaWumpa_M.LoadModel("Models/frutaWumpa.obj");
    printf("OK -> FrutaWumpa_M\n");

    printf("Cargando JoyasCrash_M -> Models/joyasCrash.obj ...\n");
    JoyasCrash_M = Model(); JoyasCrash_M.LoadModel("Models/joyasCrash.obj");
    printf("OK -> JoyasCrash_M\n");

    printf("Cargando MascaraAku_M -> Models/mascaraAku.obj ...\n");
    MascaraAku_M = Model(); MascaraAku_M.LoadModel("Models/mascaraAku.obj");
    printf("OK -> MascaraAku_M\n");

    printf("Cargando MascaraUka_M -> Models/mascaraUka.obj ...\n");
    MascaraUka_M = Model(); MascaraUka_M.LoadModel("Models/mascaraUka.obj");
    printf("OK -> MascaraUka_M\n");

    printf("Cargando Pista_M -> Models/pista.obj ...\n");
    Pista_M = Model(); Pista_M.LoadModel("Models/pista.obj");
    printf("OK -> Pista_M\n");

    printf("Cargando SteamCar1_M -> Models/steamCar1CrashandGrunt.obj ...\n");
    SteamCar1_M = Model(); SteamCar1_M.LoadModel("Models/steamCar1CrashandGrunt.obj");
    printf("OK -> SteamCar1_M\n");

    printf("Cargando SteamCar2_M -> Models/steamCar2ChiefCoco.obj ...\n");
    SteamCar2_M = Model(); SteamCar2_M.LoadModel("Models/steamCar2ChiefCoco.obj");
    printf("OK -> SteamCar2_M\n");

    printf("Cargando Totem1Crash_M -> Models/totem1Crash.obj ...\n");
    Totem1Crash_M = Model(); Totem1Crash_M.LoadModel("Models/totem1Crash.obj");
    printf("OK -> Totem1Crash_M\n");

    printf("Cargando Totem2Crash_M -> Models/totem2Crash.obj ...\n");
    Totem2Crash_M = Model(); Totem2Crash_M.LoadModel("Models/totem2Crash.obj");
    printf("OK -> Totem2Crash_M\n");

    printf("Cargando RuedaSteamCar2_M -> Models/ruedaSteamCar2ChiefCoco.obj ...\n");
    RuedaSteamCar2_M = Model(); RuedaSteamCar2_M.LoadModel("Models/ruedaSteamCar2ChiefCoco.obj");
    printf("OK -> RuedaSteamCar2_M\n");

    printf("================ FIN CARGA DE MODELOS ================\n\n");

    // ================================================
    // MATERIALES
    // ================================================
    Material_brillante = Material(4.0f, 256); // Metales, vidrio, plasmido
    Material_opaco = Material(0.3f, 4);   // Madera, tela, pasto, pared

    // ================================================
    // LUCES
    // ================================================

    // Luz direccional (sol / luna): se recalcula cada frame segun ciclo dia/noche
    mainLight = DirectionalLight(
        1.0f, 1.0f, 1.0f,
        0.3f, 0.6f,
        0.0f, -0.5f, 1.0f
    );

    unsigned int pointLightCount = 0;

    // PointLight 0: luz blanca general en el centro de la escena
    pointLights[0] = PointLight(
        1.0f, 1.0f, 1.0f,
        0.0f, 0.5f,
        0.0f, 2.0f, 0.0f,
        0.3f, 0.2f, 0.1f
    );
    pointLightCount++;

    // PointLight 1: luz roja del plasmido (posicion se actualiza cada frame)
    pointLights[1] = PointLight(
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f,
        -10.0f, -2.0f, 0.0f,
        0.5f, 0.3f, 0.2f
    );
    pointLightCount++;

    pointLights[2] = PointLight(
        0.53f, 0.81f, 0.98f,
        0.0f, 1.2f,
        62.0f, -1.0f, 18.0f,
        0.3f, 0.2f, 0.1f
    );
    pointLightCount++;

    unsigned int spotLightCount = 0;

    // SpotLight 0: linterna de la camara (flash, se actualiza cada frame)
    spotLights[0] = SpotLight(
        1.0f, 1.0f, 1.0f,
        0.0f, 2.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        5.0f
    );
    spotLightCount++;

    // SpotLight 1: luz azul estatica de la escena
    spotLights[1] = SpotLight(
        0.0f, 0.0f, 1.0f,
        1.0f, 2.0f,
        5.0f, 10.0f, 0.0f,
        0.0f, -5.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        15.0f
    );
    spotLightCount++;

    // SpotLight 2: foco derecho de la farola (se actualiza cada frame segun dia/noche)
    spotLights[2] = SpotLight(
        1.0f, 0.93f, 0.72f,
        0.0f, 0.0f,
        0.0f, -100.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        1.0f, 0.04f, 0.01f,
        40.0f
    );
    spotLightCount++;

    // SpotLight 3: foco izquierdo de la farola (se actualiza cada frame segun dia/noche)
    spotLights[3] = SpotLight(
        1.0f, 0.93f, 0.72f,
        0.0f, 0.0f,
        0.0f, -100.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        1.0f, 0.04f, 0.01f,
        40.0f
    );
    spotLightCount++;

    // ================================================
    // UNIFORMS DEL SHADER
    // ================================================
    GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0;
    GLuint uniformEyePosition = 0, uniformSpecularIntensity = 0, uniformShininess = 0;
    GLuint uniformTextureOffset = 0, uniformColor = 0;

    // Matriz de proyeccion perspectiva (se calcula una sola vez)
    glm::mat4 projection = glm::perspective(
        45.0f,
        (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(),
        0.1f,
        1000.0f
    );

    lastTime = glfwGetTime();

    // ================================================
    // VARIABLES - ESCENARIO
    // ================================================

    // Piso (mesh plano escalado)
    // Se usa meshList[0], no tiene variables propias de posicion

    // Murallas / paredes
    float      escMurallaL = 1.0f;
    glm::vec3  scaleMurallaL = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3  posMurallaL_1 = glm::vec3(-200.0f, -2.0f, -150.0f);
    glm::vec3  rotMurallaL_1 = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3  posMurallaL_2 = glm::vec3(200.0f, -2.0f, 150.0f);
    glm::vec3  rotMurallaL_2 = glm::vec3(0.0f, 180.0f, 0.0f);

    // Farola: posicion y rotacion del objeto padre
    float      escFarola = 1.0f;
    glm::vec3  posFarola = glm::vec3(6.0f, -2.0f, -4.0f);
    glm::vec3  rotFarola = glm::vec3(0.0f, 0.0f, 0.0f);
    // Offsets locales de cada pieza de la farola (relativas al padre)
    glm::vec3  offsetPaloFarola = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3  rotPaloFarola = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3  scalePaloFarola = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3  offsetFocoDerFarola = glm::vec3(0.0f, 8.38f, 3.05f);
    glm::vec3  rotFocoDerFarola = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3  scaleFocoDerFarola = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3  offsetFocoIzqFarola = glm::vec3(0.0f, 8.38f, -3.0f);
    glm::vec3  rotFocoIzqFarola = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3  scaleFocoIzqFarola = glm::vec3(1.0f, 1.0f, 1.0f);
    // Posiciones locales de la luz dentro de cada foco (para SpotLight)
    glm::vec3  luzLocalFocoDer = glm::vec3(0.0f, 8.10f, 3.05f);
    glm::vec3  luzLocalFocoIzq = glm::vec3(0.0f, 8.10f, -3.0f);
    glm::vec3  dirLocalFocoDer = glm::normalize(glm::vec3(0.0f, -1.0f, 0.0f));
    glm::vec3  dirLocalFocoIzq = glm::normalize(glm::vec3(0.0f, -1.0f, 0.0f));

    // Banca del parque
    float      escBanca = 1.0f;
    glm::vec3  posBanca = glm::vec3(2.5f, -2.0f, 6.0f);
    glm::vec3  rotBanca = glm::vec3(0.0f, 180.0f, 0.0f);
    glm::vec3  scaleBanca = glm::vec3(1.0f, 1.0f, 1.0f);

    // Bloques de pasto (12 instancias distribuidas en anillo alrededor de la escena)
    float      escBloquePasto = 1.0f;
    glm::vec3  scaleBloquePasto = glm::vec3(1.0f, 1.0f, 1.0f);
    
    glm::vec3 pastoBasePos[4] = {
        glm::vec3(160.0f, -2.0f, 115.0f),
        glm::vec3(-110.0f, -2.0f, 170.0f),
        glm::vec3(108.0f, -2.0f, -173.0f),
        glm::vec3(-160.0f, -2.0f, -120.0f)
    };

    float pastoRotY[4] = { 0.0f, -90.0f, 90.0f, 180.0f };
    float desplazamientoHaciaOrigen = 25.0f;

    glm::vec3 pastoPos[12];
    glm::vec3 pastoRot[12];
    // Anillo exterior
    for (int i = 0; i < 4; i++) {
        pastoPos[i] = pastoBasePos[i];
        pastoRot[i] = glm::vec3(0.0f, pastoRotY[i], 0.0f);
    }
    // Anillo medio
    for (int i = 0; i < 4; i++) {
        pastoPos[i + 4] = MoveTowardsOriginXZ(pastoBasePos[i], desplazamientoHaciaOrigen);
        pastoRot[i + 4] = glm::vec3(0.0f, pastoRotY[i], 0.0f);
    }
    // Anillo interior
    for (int i = 0; i < 4; i++) {
        pastoPos[i + 8] = MoveTowardsOriginXZ(pastoBasePos[i], desplazamientoHaciaOrigen * 2.0f);
        pastoRot[i + 8] = glm::vec3(0.0f, pastoRotY[i], 0.0f);
    }

    // Reloj de planetas (objeto padre + piezas jerarquicas)
    float      escReloj = 4.0f;
    glm::vec3  posReloj = glm::vec3(0.0f, -2.0f, -20.0f);
    glm::vec3  rotReloj = glm::vec3(0.0f, 0.0f, 0.0f);
    // Offsets y escalas locales de cada pieza del reloj
    glm::vec3  offsetBasePlanetas = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3  rotBasePlanetas = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3  scaleBasePlanetas = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3  offsetRelojBase = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3  rotRelojBase = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3  scaleRelojBase = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3  offsetRelojEngrane1 = glm::vec3(0.0f, 0.3f, 0.3f);
    glm::vec3  rotRelojEngrane1 = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3  scaleRelojEngrane1 = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3  offsetRelojEngrane2 = glm::vec3(0.0f, 0.3f, 0.0f);
    glm::vec3  rotRelojEngrane2 = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3  scaleRelojEngrane2 = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3  offsetRelojEngrane3 = glm::vec3(0.0f, 0.1f, 0.0f);
    glm::vec3  rotRelojEngrane3 = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3  scaleRelojEngrane3 = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3  offsetRelojEngrane4 = glm::vec3(0.0f, 0.1f, 0.0f);
    glm::vec3  rotRelojEngrane4 = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3  scaleRelojEngrane4 = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3  offsetRelojGalRedE = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3  rotRelojGalRedE = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3  scaleRelojGalRedE = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3  offsetRelojPlanetas = glm::vec3(0.1f, 2.2f, -0.1f);
    glm::vec3  rotRelojPlanetas = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3  scaleRelojPlanetas = glm::vec3(1.0f, 1.0f, 1.0f);
    // Velocidades de orbita de cada engrane del reloj (grados/segundo)
    float velRelojEngrane1 = 15.0f;
    float velRelojEngrane2 = 6.0f;
    float velRelojEngrane3 = 6.0f;
    float velRelojEngrane4 = 6.0f;
    float velRelojGalRedE = 6.0f;
    float velRelojPlanetas = 6.0f;
    // Angulos de orbita acumulados (se actualizan cada frame)
    float angOrbitaRelojEngrane1 = 0.0f;
    float angOrbitaRelojEngrane2 = 0.0f;
    float angOrbitaRelojEngrane3 = 0.0f;
    float angOrbitaRelojEngrane4 = 0.0f;
    float angOrbitaRelojGalRedE = 0.0f;
    float angOrbitaRelojPlanetas = 0.0f;

    // Ciclo dia/noche
    float duracionDia = 1800.0f; // frames que dura el dia
    float duracionTrans = 7.0f;     // frames que dura la transicion
    int   estadoCiclo = 0;        // 0=dia, 1=atardecer, 2=noche, 3=amanecer
    float ciclTimer = 0.0f;
    bool  prevTeclaT = false;    // tecla T alterna el ciclo manualmente

    // Debug: timer para imprimir posicion de camara en consola
    float debugTimer = 0.0f;

    // ============================================
    // CAMARAS - HARRY POTTER / HERMIONE
    // ============================================

    int modoCamaraHP = 0;
    bool prevF1Cam = false;
    bool prevF2Cam = false;
    bool prevF3Cam = false;
    bool hpCamInit = false;

    // Camara de tercera persona ligada a Hermione
    float hpCamDistTP = 6.0f;
    float hpCamAlturaTP = 3.0f;
    float hpCamLookAhead = 1.5f;
    float hpCamLookY = 1.2f;

    // Camara aerea sobre plano XZ
    glm::vec3 hpCamAereaPos(0.0f, 0.0f, 0.0f);
    float hpCamAereaVel = 12.0f;
    float hpCamAereaAltura = 16.0f;

    // Camara de recorrido de interes
    std::vector<glm::vec3> hpCamInteresPos;
    std::vector<glm::vec3> hpCamInteresTarget;
    float hpCamInteresTimer = 0.0f;
    float hpCamInteresDuracion = 4.0f;


    // ================================================
    // VARIABLES - AVATAR BIOSHOCK: Big Daddy
    // ================================================

    // Escala global del Big Daddy
    float      escBigDaddy = 2.0f;
    // Posicion del objeto padre del Big Daddy en la escena
    glm::vec3  posBigDaddy = glm::vec3(0.0f, -1.0f, 0.0f);
    // Offsets locales de cada parte del Big Daddy (relativas al padre)
    glm::vec3  offsetCuerpo = glm::vec3(-0.9f, 4.0f, 1.0f);
    glm::vec3  rotCuerpo = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3  offsetBrazoIzq = glm::vec3(-2.0f, 4.2f, 0.0f);
    glm::vec3  rotBrazoIzq = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3  offsetBrazoDer = glm::vec3(0.2f, 4.2f, 0.0f);
    glm::vec3  rotBrazoDer = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3  offsetPiernaIzq = glm::vec3(-1.5f, 2.7f, 0.0f);
    glm::vec3  rotPiernaIzq = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3  offsetPiernaDer = glm::vec3(-0.15f, 2.7f, 0.0f);
    glm::vec3  rotPiernaDer = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3  offsetTaladro = glm::vec3(1.6f, 0.7f, 0.1f);
    glm::vec3  rotTaladro = glm::vec3(0.0f, 0.0f, 45.0f);
    // Animacion del taladro (tecla 1)
    float      taladroRot = 1.0f;
    bool       taladroGirando = false;
    bool       prevTecla1 = false;
    // Animacion de caminata del Big Daddy automatica por trayectoria cerrada
    float caminataTime = 0.0f;
    float caminataSpeed = 3.0f;
    float caminataAmp = 25.0f;         // amplitud piernas
    float caminataBrazoAmp = 10.0f;    // amplitud brazos
    bool bigDaddyCaminando = true;

    // Posicion y orientacion real del Big Daddy sobre el mundo
    glm::vec3 bigDaddyPosActual = glm::vec3(-100.31f, posBigDaddy.y, 108.96f);
    float bigDaddyRotY = 0.0f;
    float bigDaddyVel = 9.0f;

    // Ruta cerrada: se ignora Y de tus capturas y se conserva la Y actual del Big Daddy
    std::vector<glm::vec3> rutaBigDaddy = {
        glm::vec3(-100.31f, posBigDaddy.y, 108.96f),
        glm::vec3(-101.90f, posBigDaddy.y, -113.97f),
        glm::vec3(-94.94f,  posBigDaddy.y, -113.87f),
        glm::vec3(8.21f,  posBigDaddy.y, -113.42f),
        glm::vec3(1.95f,  posBigDaddy.y,  -62.86f),
        glm::vec3(25.05f,  posBigDaddy.y,  -66.81f),
        glm::vec3(102.11f,  posBigDaddy.y, -117.19f),
        glm::vec3(104.51f,  posBigDaddy.y, -103.92f),
        glm::vec3(102.80f,  posBigDaddy.y,   -6.70f),
        glm::vec3(71.21f,  posBigDaddy.y,    7.65f),
        glm::vec3(98.84f,  posBigDaddy.y,   64.48f),
        glm::vec3(104.10f,  posBigDaddy.y,  108.58f),
        glm::vec3(61.31f,  posBigDaddy.y,  106.53f),
        glm::vec3(-17.02f,  posBigDaddy.y,  109.65f),
        glm::vec3(-17.15f,  posBigDaddy.y,   65.64f),
        glm::vec3(-49.46f,  posBigDaddy.y,    3.16f),
        glm::vec3(56.64f,  posBigDaddy.y,   -0.67f),
        glm::vec3(-25.46f,  posBigDaddy.y,   70.07f),
        glm::vec3(-101.35f, posBigDaddy.y,  112.95f)
    };

    int bigDaddyNodoActual = 1;
    // Efecto de vibracion de camara al acercarse al Big Daddy
    float      vibracionAmp = 0.05f;
    float      vibracionRangoMax = 15.0f;

    // ================================================
    // VARIABLES - OBJETOS BIOSHOCK
    // ================================================

    // Medkit (bolsa): escala y posicion del padre, offsets de tapas
    float      escMedkit = 0.25f;
    glm::vec3  posMedkit = glm::vec3(10.0f, -1.0f, 0.0f);
    glm::vec3  offsetBolsaDer = glm::vec3(0.0f, 0.2f, -0.13f);
    glm::vec3  offsetBolsaIzq = glm::vec3(0.0f, 0.0f, 1.1f);
    // Animacion de apertura del medkit (se activa al acercarse)
    float      puertaAngulo = 0.0f;
    float      puertaTarget = 0.0f;
    float      puertaSpeed = 3.0f;
    float      distActivacionMedkit = 5.0f;

    // Plasmido: escala base, posicion y animaciones de flotacion/latido/rotacion
    float      escPlasmido = 3.0f;
    glm::vec3  posPlasmido = glm::vec3(-10.0f, -1.0f, 0.0f);
    float      plasTime = 0.0f;
    float      plasFloatSpeed = 0.25f;  // velocidad del seno de flotacion
    float      plasFloatAmp = 0.04f;  // amplitud de flotacion en Y
    float      plasRotSpeed = 3.0f;   // velocidad de rotacion en Y
    float      plasRotY = 0.0f;   // angulo de rotacion acumulado
    float      plasHeartSpeed = 0.4f;   // velocidad del "latido" de escala
    float      plasHeartAmp = 0.25f;  // amplitud del latido de escala


    //copa animacion
    float copaTime = 0.0f;
    float copaFloatSpeed = 0.25f;
    float copaFloatAmp = 0.04f;
    float copaRotSpeed = 3.0f;
    float copaRotY = 0.0f;
    float copaHeartSpeed = 0.4f;
    float copaHeartAmp = 0.20f;

    // ================================================
    // VARIABLES - AVATAR HARRY POTTER: Hermione
    // ================================================

    // Offset de toda la escena de Harry Potter en el mundo
    glm::vec3 hpOffsetEscena = glm::vec3(55.0f, 0.0f, 25.0f);

    // Posicion y rotacion de Hermione (movimiento con FLECHAS)
    float hpPosX = 0.0f;
    float hpPosZ = 0.0f;
    float hpRotPersonaje = 0.0f;
    float hpVelocidad = 10.0f;

    // Angulos de animacion de extremidades de Hermione
    float hpRotBrazoDer = 0.0f;
    float hpRotBrazoIzq = 0.0f;
    float hpRotPiernaDer = 0.0f;
    float hpRotPiernaIzq = 0.0f;


    // ================================================
  // VARIABLES - OBJETOS HARRY POTTER
  // ================================================

  // Carro volador: ruta automatica con tecla 1
    std::vector<glm::vec3> hpCarroRuta = {
        glm::vec3(-125.04f, 4.45f, -129.69f),
        glm::vec3(-87.08f,  5.65f, -158.69f),
        glm::vec3(114.75f,  2.43f, -153.83f),
        glm::vec3(129.78f,  1.60f, -102.01f),
        glm::vec3(129.45f,  1.62f,  95.31f),
        glm::vec3(103.59f,  1.86f, 130.33f),
        glm::vec3(-84.61f,  1.87f, 135.58f),
        glm::vec3(-123.09f, 1.18f, 110.57f),
        glm::vec3(-129.73f, 1.57f, -93.24f),
        glm::vec3(-118.50f, 1.81f, -149.37f),
        glm::vec3(-84.57f,  2.61f, -165.86f)
    };

    float hpCarroYFijo = 1.60f;
    glm::vec3 hpCarroPos = glm::vec3(hpCarroRuta[0].x, hpCarroYFijo, hpCarroRuta[0].z);
    float hpRotCarro = 0.0f;
    float hpRotLlantas = 0.0f;
    float hpVelocidadCarro = 10.0f;
    bool hpCarroAutoActivo = false;
    bool prevTeclaAuto1 = false;
    int hpCarroWPActual = 1;

    // Props de Harry Potter
    glm::vec3 hpPosBolsa = hpOffsetEscena + glm::vec3(-8.0f, -1.0f, -4.0f);
    glm::vec3 hpPosGiratiempo = hpOffsetEscena + glm::vec3(-10.0f, 0.0f, -7.0f);
    glm::vec3 hpPosLibro = hpOffsetEscena + glm::vec3(3.0f, -1.0f, 3.0f);
    glm::vec3 hpPosCopa = hpOffsetEscena + glm::vec3(7.0f, -1.0f, -7.0f);

    // Snitch: ruta automatica con tecla 3
    std::vector<glm::vec3> hpSnitchRuta = {
        glm::vec3(99.43f,   5.88f, -32.88f),
        glm::vec3(49.79f,   3.06f,  47.08f),
        glm::vec3(31.90f,   5.82f, 102.71f),
        glm::vec3(-11.96f,  2.07f,  19.60f),
        glm::vec3(-11.74f,  3.85f, -27.56f),
        glm::vec3(17.69f,   4.55f, -103.31f),
        glm::vec3(-97.00f,  3.90f, -107.43f),
        glm::vec3(-98.47f,  6.74f,  -5.85f),
        glm::vec3(-99.56f,  4.43f,  85.56f),
        glm::vec3(33.56f,   4.25f,   4.02f)
    };

    glm::vec3 hpPosSnitch = hpSnitchRuta[0];
    bool hpSnitchVisible = true;
    bool hpSnitchAnimacionActiva = false;
    bool prevTeclaAuto3 = false;
    int hpSnitchWPActual = 1;
    float hpSnitchVelocidadRuta = 15.0f;

    // Expreso de Hogwarts: posiciones fijas
    glm::vec3 posCabinaExpreso = hpOffsetEscena + glm::vec3(0.0f, -2.0f, 15.0f);
    glm::vec3 posVagonExpreso = hpOffsetEscena + glm::vec3(5.0f, -2.0f, 15.0f);
    glm::vec3 posBielaIzqExpreso = hpOffsetEscena + glm::vec3(10.0f, -2.0f, 15.0f);
    glm::vec3 posBielaMotrizIzq = hpOffsetEscena + glm::vec3(15.0f, -2.0f, 15.0f);
    glm::vec3 posEjeDelanExpreso = hpOffsetEscena + glm::vec3(20.0f, -2.0f, 15.0f);
    glm::vec3 posEjeTrasExpreso = hpOffsetEscena + glm::vec3(25.0f, -2.0f, 15.0f);
    glm::vec3 posEjeVagonExpreso = hpOffsetEscena + glm::vec3(30.0f, -2.0f, 15.0f);
    glm::vec3 posPistonIzqExpreso = hpOffsetEscena + glm::vec3(35.0f, -2.0f, 15.0f);
    glm::vec3 posLlantaSteamCar3 = hpOffsetEscena + glm::vec3(40.0f, -2.0f, 15.0f);
    glm::vec3 posSteamCar3 = hpOffsetEscena + glm::vec3(45.0f, -2.0f, 15.0f);


    // ================================================
    // VARIABLES - AVATAR CRASH BANDICOOT: Crash
    // ================================================

    glm::vec3 crashOffsetEscena = glm::vec3(-55.0f, 0.0f, 25.0f);

    float crashPosX = 0.0f;
    float crashPosZ = 0.0f;
    float crashRotY = 0.0f;
    float crashVelocidad = 2.0f;

    float crashRotBrazoDer = 0.0f;
    float crashRotBrazoIzq = 0.0f;
    float crashRotPiernaDer = 0.0f;
    float crashRotPiernaIzq = 0.0f;


    // ================================================
    // VARIABLES - OBJETOS CRASH BANDICOOT
    // ================================================

    // Go-kart de Crash: ruta automatica con tecla 2
    std::vector<glm::vec3> kartRuta = {
        glm::vec3(-53.65f,  2.59f, -151.59f),
        glm::vec3(-79.39f,  2.78f, -152.69f),
        glm::vec3(-116.51f, 1.83f, -129.59f),
        glm::vec3(-120.70f, 2.37f,   76.67f),
        glm::vec3(-103.70f, 2.25f,  117.08f),
        glm::vec3(90.09f,   2.16f,  121.69f),
        glm::vec3(119.34f,  2.59f,   96.96f),
        glm::vec3(120.59f,  2.58f, -113.85f),
        glm::vec3(95.87f,   2.86f, -148.83f),
        glm::vec3(-35.04f,  2.35f, -150.78f)
    };

    glm::vec3 kartPos = kartRuta[0];
    float kartRotY = 0.0f;
    float kartVelocidad = 8.0f;
    float kartRotLlantas = 0.0f;
    bool kartAutoActivo = false;
    bool prevTeclaAuto2 = false;
    int kartWPActual = 1;

    // Steam car de Crash (decorativo, posicion fija)
    glm::vec3 posSteamCar = crashOffsetEscena + glm::vec3(8.0f, -1.5f, -8.0f);

    // Posiciones de los props del universo Crash (linea de display en Z+15)
    glm::vec3  posArbolWumpa = crashOffsetEscena + glm::vec3(0.0f, -2.0f, 15.0f);
    glm::vec3  posBaseCilindro = crashOffsetEscena + glm::vec3(5.0f, -2.0f, 15.0f);
    glm::vec3  posBolaRoca = crashOffsetEscena + glm::vec3(10.0f, -2.0f, 15.0f);
    glm::vec3  posCajaAku = crashOffsetEscena + glm::vec3(15.0f, -2.0f, 15.0f);
    glm::vec3  posCajaCaraCrash = crashOffsetEscena + glm::vec3(20.0f, -2.0f, 15.0f);
    glm::vec3  posCajaCheckPoint = crashOffsetEscena + glm::vec3(25.0f, -2.0f, 15.0f);
    glm::vec3  posCajaFlechaArriba = crashOffsetEscena + glm::vec3(30.0f, -2.0f, 15.0f);
    glm::vec3  posCajaMetalExcl = crashOffsetEscena + glm::vec3(35.0f, -2.0f, 15.0f);
    glm::vec3  posCajaNitro = crashOffsetEscena + glm::vec3(40.0f, -2.0f, 15.0f);
    glm::vec3  posCajaNitroExcl = crashOffsetEscena + glm::vec3(45.0f, -2.0f, 15.0f);
    glm::vec3  posCajaNormal = crashOffsetEscena + glm::vec3(50.0f, -2.0f, 15.0f);
    glm::vec3  posCajaQuestion = crashOffsetEscena + glm::vec3(55.0f, -2.0f, 15.0f);
    glm::vec3  posCajaTNT = crashOffsetEscena + glm::vec3(60.0f, -2.0f, 15.0f);
    glm::vec3  posCocoOnGoKart = crashOffsetEscena + glm::vec3(65.0f, -2.0f, 15.0f);
    glm::vec3  posCortexOnGoKart = crashOffsetEscena + glm::vec3(70.0f, -2.0f, 15.0f);
    glm::vec3  posCrystalCrash = crashOffsetEscena + glm::vec3(75.0f, -2.0f, 15.0f);
    glm::vec3  posCrystalHielo = crashOffsetEscena + glm::vec3(80.0f, -2.0f, 15.0f);
    glm::vec3  posFrutaWumpa = crashOffsetEscena + glm::vec3(85.0f, -2.0f, 15.0f);
    glm::vec3  posJoyasCrash = crashOffsetEscena + glm::vec3(90.0f, -2.0f, 15.0f);
    glm::vec3  posMascaraAku = crashOffsetEscena + glm::vec3(95.0f, -2.0f, 15.0f);
    glm::vec3  posMascaraUka = crashOffsetEscena + glm::vec3(100.0f, -2.0f, 15.0f);
    glm::vec3 posPista = glm::vec3(0.0f, -1.0f, 8.0f);
    glm::vec3  posSteamCar1 = crashOffsetEscena + glm::vec3(110.0f, -2.0f, 15.0f);
    glm::vec3  posSteamCar2 = crashOffsetEscena + glm::vec3(115.0f, -2.0f, 15.0f);
    glm::vec3  posTotem1Crash = crashOffsetEscena + glm::vec3(120.0f, -2.0f, 15.0f);
    glm::vec3  posTotem2Crash = crashOffsetEscena + glm::vec3(125.0f, -2.0f, 15.0f);
    glm::vec3  posRuedaSteamCar2 = crashOffsetEscena + glm::vec3(130.0f, -2.0f, 15.0f);


    // ================================================
    // VARIABLES - SISTEMA DE CAMARAS
    // ================================================
    int modoCamara = 1;           // 1=3raPersona, 2=Aerea, 3=Interes
    bool prevTeclaVirgula = false; // tecla ,
    bool prevTeclaPunto = false;   // tecla .
    bool prevTeclaMenos = false;   // tecla -
    int puntoInteresActual = 0;
    bool prevTeclaV = false;


    // ================================================
    // LOOP PRINCIPAL
    // ================================================
    while (!mainWindow.getShouldClose())
    {
        // --- Delta time ---
        GLfloat now = glfwGetTime();
        deltaTime = now - lastTime;
        if (deltaTime > 0.05f) deltaTime = 0.05f; // cap a 20fps minimo
        lastTime = now;

        glfwPollEvents();
        // Camara libre siempre procesa mouse, WASD solo en modo libre
        camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
        if (modoCamara == 3)
        {
            camera.keyControl(mainWindow.getsKeys(), deltaTime);
        }
        // --- Debug: posicion e instruccion de captura en consola ---
        debugTimer += deltaTime;
        if (debugTimer >= 0.05f)
        {
            debugTimer = 0.0f;
            glm::vec3 pos = camera.getCameraPosition();
            glm::vec3 dir = camera.getCameraDirection();
            printf("\r  POS( x:% 7.2f  y:% 7.2f  z:% 7.2f )   DIR( x:% 5.2f  y:% 5.2f  z:% 5.2f )   [C=captura]  ",
                pos.x, pos.y, pos.z, dir.x, dir.y, dir.z);
            fflush(stdout);
        }

        // --- Modo captura de waypoints ---
        bool teclaCActual = mainWindow.getsKeys()[GLFW_KEY_C];
        if (teclaCActual && !teclaCAnterior)
        {
            modoCaptura = !modoCaptura;
            if (modoCaptura) {
                printf("\n--- MODO CAPTURA ACTIVADO ---\n");
                printf("P = guardar punto    L = limpiar    C = salir\n");
                capturaContador = 0;
                waypointsCapturados.clear();
            }
            else {
                printf("\n--- MODO CAPTURA DESACTIVADO --- total: %d puntos\n", capturaContador);
            }
        }
        teclaCAnterior = teclaCActual;

        bool teclaPActual = mainWindow.getsKeys()[GLFW_KEY_P];
        if (teclaPActual && !teclaPAnterior && modoCaptura)
        {
            glm::vec3 cp = camera.getCameraPosition();
            glm::vec3 punto = glm::vec3(cp.x, cp.y - 1.5f, cp.z);
            waypointsCapturados.push_back(punto);
            printf("\n\tglm::vec3(%.2ff, %.2ff, %.2ff),   // punto %d\n",
                punto.x, punto.y, punto.z, capturaContador + 1);
            fflush(stdout);
            capturaContador++;
        }
        teclaPAnterior = teclaPActual;

        bool teclaLActual = mainWindow.getsKeys()[GLFW_KEY_L];
        if (teclaLActual && !teclaLAnterior && modoCaptura)
        {
            printf("\nLista limpiada, empezando de nuevo\n");
            capturaContador = 0;
            waypointsCapturados.clear();
        }
        teclaLAnterior = teclaLActual;

        // ============================================
        // LOGICA DE ANIMACIONES Y CONTROLES
        // ============================================

        //copa animacion como plasmido
        copaTime += deltaTime;
        copaRotY += copaRotSpeed * deltaTime;
        if (copaRotY > 360.0f) copaRotY -= 360.0f;

        float copaOffsetY = copaFloatAmp * sinf(copaTime * copaFloatSpeed);
        float copaHeartbeat = fabsf(sinf(copaTime * copaHeartSpeed));
        float copaEscActual = 1.0f + copaHeartAmp * copaHeartbeat;

        // --- Taladro del Big Daddy automatico mientras camina ---
        if (bigDaddyCaminando)
        {
            taladroRot += 150.0f * deltaTime;
            if (taladroRot > 360.0f)
                taladroRot -= 360.0f;
        }

        // --- Caminata automatica del Big Daddy por ruta cerrada ---
        float pasoSeno = 0.0f;
        float anguloPiernaIzq = 0.0f, anguloPiernaDer = 0.0f;
        float anguloBrazoIzq = 0.0f, anguloBrazoDer = 0.0f;

        if (rutaBigDaddy.size() >= 2)
        {
            glm::vec3 destino = rutaBigDaddy[bigDaddyNodoActual];
            glm::vec3 direccion = destino - bigDaddyPosActual;
            direccion.y = 0.0f;

            float distancia = glm::length(direccion);

            if (distancia > 0.001f)
            {
                bigDaddyCaminando = true;

                glm::vec3 dirNorm = glm::normalize(direccion);
                float paso = bigDaddyVel * deltaTime;

                if (paso >= distancia)
                {
                    bigDaddyPosActual = destino;
                    bigDaddyNodoActual = (bigDaddyNodoActual + 1) % rutaBigDaddy.size();
                }
                else
                {
                    bigDaddyPosActual += dirNorm * paso;
                }

                bigDaddyPosActual.y = posBigDaddy.y;

                // Ajuste para que el modelo mire hacia donde avanza
                bigDaddyRotY = -glm::degrees(atan2(dirNorm.x, dirNorm.z)) + 180.0f;

                // Reutiliza la animacion que ya tenias en brazos y piernas
                caminataTime += deltaTime;
                pasoSeno = sinf(caminataTime * caminataSpeed);

                anguloPiernaIzq = pasoSeno * caminataAmp;
                anguloPiernaDer = -pasoSeno * caminataAmp;
                anguloBrazoIzq = -pasoSeno * caminataBrazoAmp;
                anguloBrazoDer = pasoSeno * caminataBrazoAmp;
            }
            else
            {
                bigDaddyPosActual = destino;
                bigDaddyNodoActual = (bigDaddyNodoActual + 1) % rutaBigDaddy.size();
            }
        }
        else
        {
            bigDaddyCaminando = false;
            bigDaddyPosActual = posBigDaddy;
            bigDaddyRotY = 0.0f;
        }

        // --- Apertura del medkit al acercarse ---
        float distCamMK = glm::length(camera.getCameraPosition() - posMedkit);
        puertaTarget = (distCamMK <= distActivacionMedkit) ? 45.0f : 0.0f;
        if (puertaAngulo < puertaTarget) {
            puertaAngulo += puertaSpeed * deltaTime;
            if (puertaAngulo > puertaTarget) puertaAngulo = puertaTarget;
        }
        else if (puertaAngulo > puertaTarget) {
            puertaAngulo -= puertaSpeed * deltaTime;
            if (puertaAngulo < puertaTarget) puertaAngulo = puertaTarget;
        }


        bool* keys = mainWindow.getsKeys();

        // ============================================
                // CONTROLES - SISTEMA DE CAMARAS (coma=modo1, punto=modo2, menos=modo3)
                // ============================================

        bool currVirgula = keys[GLFW_KEY_COMMA];
        bool currPunto = keys[GLFW_KEY_PERIOD];
        bool currM = keys[GLFW_KEY_M];

        if (currVirgula && !prevTeclaVirgula) {
            modoCamara = 1;
            printf("Camara: Primera Persona (Hermione)\n");
        }
        if (currPunto && !prevTeclaPunto) {
            modoCamara = 2;
            printf("Camara: Tercera Persona (Hermione)\n");
        }
        if (currM && !prevTeclaMenos) {
            modoCamara = 3;
            printf("Camara: Libre (WASD + mouse)\n");
        }
        prevTeclaVirgula = currVirgula;
        prevTeclaPunto = currPunto;
        prevTeclaMenos = currM;

        // Camara modo 3: avanzar punto de interes con V
        if (modoCamara == 3) {
            bool currV = keys[GLFW_KEY_V];
            if (currV && !prevTeclaV) {
                puntoInteresActual = (puntoInteresActual + 1) % 3;
                printf("Punto de interes: %d\n", puntoInteresActual);
            }
            prevTeclaV = keys[GLFW_KEY_V];
            hpCamInteresTimer += deltaTime;
            float cicloTotal = hpCamInteresDuracion * (float)hpCamInteresPos.size();
            if (hpCamInteresTimer >= cicloTotal) hpCamInteresTimer = 0.0f;
        }

        // Camara aerea modo 2: desplazamiento WASD
        if (modoCamara == 2) {
            if (keys[GLFW_KEY_W]) hpCamAereaPos.z -= hpCamAereaVel * deltaTime;
            if (keys[GLFW_KEY_S]) hpCamAereaPos.z += hpCamAereaVel * deltaTime;
            if (keys[GLFW_KEY_A]) hpCamAereaPos.x -= hpCamAereaVel * deltaTime;
            if (keys[GLFW_KEY_D]) hpCamAereaPos.x += hpCamAereaVel * deltaTime;
            hpCamAereaPos.y = hpOffsetEscena.y + hpCamAereaAltura;
        }

        // Mantener compatibilidad con modoCamaraHP
        modoCamaraHP = modoCamara - 1;
        if (modoCamaraHP < 0) modoCamaraHP = 0;

        // --- Ciclo dia/noche (tecla T: avanza manualmente) ---
        bool currTeclaT = mainWindow.getsKeys()[GLFW_KEY_T];
        if (currTeclaT && !prevTeclaT) { estadoCiclo = (estadoCiclo + 1) % 4; ciclTimer = 0.0f; }
        prevTeclaT = currTeclaT;

        ciclTimer += deltaTime;
        switch (estadoCiclo) {
        case 0: if (ciclTimer >= duracionDia) { estadoCiclo = 1; ciclTimer = 0.0f; } break;
        case 1: if (ciclTimer >= duracionTrans) { estadoCiclo = 2; ciclTimer = 0.0f; } break;
        case 2: if (ciclTimer >= duracionDia) { estadoCiclo = 3; ciclTimer = 0.0f; } break;
        case 3: if (ciclTimer >= duracionTrans) { estadoCiclo = 0; ciclTimer = 0.0f; } break;
        }

        float factorDia = 1.0f;
        switch (estadoCiclo) {
        case 0: factorDia = 1.0f; break;
        case 1: factorDia = 1.0f - (ciclTimer / duracionTrans); break;
        case 2: factorDia = 0.0f; break;
        case 3: factorDia = ciclTimer / duracionTrans; break;
        }

        float ambientDia = 0.3f, diffuseDia = 0.8f;
        float ambientNoche = 0.05f, diffuseNoche = 0.1f;
        float ambientActual = ambientNoche + (ambientDia - ambientNoche) * factorDia;
        float diffuseActual = diffuseNoche + (diffuseDia - diffuseNoche) * factorDia;
        mainLight = DirectionalLight(1.0f, 1.0f, 1.0f, ambientActual, diffuseActual, 0.0f, -0.5f, 1.0f);

        // --- Animacion del plasmido (flotacion + latido + rotacion) ---
        plasTime += deltaTime;
        plasRotY += plasRotSpeed * deltaTime;
        if (plasRotY >= 360.0f) plasRotY -= 360.0f;
        float plasOffsetY = plasFloatAmp * sinf(plasTime * plasFloatSpeed);
        float heartbeat = fabsf(sinf(plasTime * plasHeartSpeed));
        float plasEscActual = escPlasmido + plasHeartAmp * heartbeat;

        pointLights[1] = PointLight(
            1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
            posPlasmido.x, posPlasmido.y + plasOffsetY, posPlasmido.z,
            0.5f, 0.3f, 0.2f
        );

        pointLights[2] = PointLight(
            0.53f, 0.81f, 0.98f,
            0.0f, 1.2f,
            hpPosCopa.x, hpPosCopa.y + copaOffsetY, hpPosCopa.z,
            0.3f, 0.2f, 0.1f
        );

        // ============================================
        // VIEW MATRIX SEGUN CAMARA ACTIVA DE HERMIONE
        // ============================================

        glm::vec3 hpWorldPos = hpOffsetEscena + glm::vec3(hpPosX, 1.0f, hpPosZ);
        float hpYawRad = hpRotPersonaje * toRadians;
        glm::vec3 hpForward = glm::normalize(glm::vec3(sinf(hpYawRad), 0.0f, cosf(hpYawRad)));

        glm::vec3 camPosActiva(0.0f, 0.0f, 0.0f);
        glm::vec3 camTargetActiva(0.0f, 0.0f, 0.0f);
        glm::vec3 camDirActiva(0.0f, 0.0f, -1.0f);

        if (modoCamara == 1)
        {
            // Primera persona: camara en la cabeza de Hermione, adelantada para no ver el modelo
            camPosActiva = hpWorldPos + glm::vec3(0.0f, 1.6f, 0.0f) + hpForward * 0.7f;
            camTargetActiva = camPosActiva + hpForward * 5.0f;
        }
        else if (modoCamara == 2)
        {
            // Tercera persona: camara detras y arriba mirando a Hermione
            camPosActiva = hpWorldPos - hpForward * hpCamDistTP + glm::vec3(0.0f, hpCamAlturaTP, 0.0f);
            camTargetActiva = hpWorldPos + glm::vec3(0.0f, hpCamLookY, 0.0f) + hpForward * hpCamLookAhead;
        }
        else if (modoCamara == 3)
        {
            // Camara libre: WASD ya se aplico arriba, solo leer posicion
            camPosActiva = camera.getCameraPosition();
            camTargetActiva = camPosActiva + camera.getCameraDirection() * 5.0f;
        }

        else
        {

            // Puntos de interes: recorre 3 elementos del escenario HP
            int n = (int)hpCamInteresPos.size();
            int tramo = (int)(hpCamInteresTimer / hpCamInteresDuracion);
            if (tramo >= n) tramo = 0;

            int siguiente = (tramo + 1) % n;
            float t = (hpCamInteresTimer - (float)tramo * hpCamInteresDuracion) / hpCamInteresDuracion;

            camPosActiva = glm::mix(hpCamInteresPos[tramo], hpCamInteresPos[siguiente], t);
            camTargetActiva = glm::mix(hpCamInteresTarget[tramo], hpCamInteresTarget[siguiente], t);
        }

        camDirActiva = glm::normalize(camTargetActiva - camPosActiva);

        // --- Vibracion de camara por proximidad al Big Daddy ---
        float distCamBD = glm::length(camPosActiva - bigDaddyPosActual);
        float factorCercania = 1.0f - (distCamBD / vibracionRangoMax);
        if (factorCercania < 0.0f) factorCercania = 0.0f;

        float vibX = sinf(caminataTime * caminataSpeed * 2.0f) * vibracionAmp * factorCercania;
        float vibY = fabsf(sinf(caminataTime * caminataSpeed * 2.0f)) * vibracionAmp * factorCercania;

        // En modo libre NO sobreescribir la camara, ya tiene su posicion de WASD
        if (modoCamara != 3)
        {
            camera.setPosition(camPosActiva);
            camera.lookAt(camTargetActiva);
        }
        else
        {
            // En modo libre, recalcular camPosActiva/Target desde la camara actual
            camPosActiva = camera.getCameraPosition();
            camTargetActiva = camPosActiva + camera.getCameraDirection() * 5.0f;
            camDirActiva = camera.getCameraDirection();
        }

        glm::mat4 viewMatrix = glm::lookAt(
            camPosActiva,
            camTargetActiva,
            glm::vec3(0.0f, 1.0f, 0.0f)
        );
        viewMatrix = glm::translate(viewMatrix, glm::vec3(vibX, vibY, 0.0f));

        glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(viewMatrix));
        glUniform3f(uniformEyePosition, camPosActiva.x, camPosActiva.y, camPosActiva.z);

        glm::vec3 lowerLight = camPosActiva;
        lowerLight.y -= 0.3f;
        spotLights[0].SetFlash(lowerLight, camDirActiva);



        // --- Engranes del reloj (orbita continua cada frame) ---
        angOrbitaRelojEngrane1 -= velRelojEngrane1 * deltaTime;
        angOrbitaRelojEngrane2 += velRelojEngrane2 * deltaTime;
        angOrbitaRelojEngrane3 -= velRelojEngrane3 * deltaTime;
        angOrbitaRelojEngrane4 += velRelojEngrane4 * deltaTime;
        angOrbitaRelojGalRedE -= velRelojGalRedE * deltaTime;
        angOrbitaRelojPlanetas -= velRelojPlanetas * deltaTime;
        if (angOrbitaRelojEngrane1 <= -360.0f) angOrbitaRelojEngrane1 += 360.0f;
        if (angOrbitaRelojEngrane2 >= 360.0f) angOrbitaRelojEngrane2 -= 360.0f;
        if (angOrbitaRelojEngrane3 <= -360.0f) angOrbitaRelojEngrane3 += 360.0f;
        if (angOrbitaRelojEngrane4 >= 360.0f) angOrbitaRelojEngrane4 -= 360.0f;
        if (angOrbitaRelojGalRedE <= -360.0f) angOrbitaRelojGalRedE += 360.0f;
        if (angOrbitaRelojPlanetas <= -360.0f) angOrbitaRelojPlanetas += 360.0f;

        // --- Farola: calcular posicion mundial de los focos para SpotLights ---
        glm::mat4 baseFarola = glm::mat4(1.0f);
        baseFarola = glm::translate(baseFarola, posFarola);
        baseFarola = glm::rotate(baseFarola, rotFarola.x * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
        baseFarola = glm::rotate(baseFarola, rotFarola.y * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        baseFarola = glm::rotate(baseFarola, rotFarola.z * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));

        glm::vec3 focoDerWorld = glm::vec3(baseFarola * glm::vec4(luzLocalFocoDer, 1.0f));
        glm::vec3 focoIzqWorld = glm::vec3(baseFarola * glm::vec4(luzLocalFocoIzq, 1.0f));
        glm::vec3 dirFocoDerWorld = glm::normalize(glm::mat3(baseFarola) * dirLocalFocoDer);
        glm::vec3 dirFocoIzqWorld = glm::normalize(glm::mat3(baseFarola) * dirLocalFocoIzq);

        // La farola solo se enciende de noche (factorDia < 0.5)
        bool  farolaEncendida = (factorDia < 0.5f);
        float ambientFarola = farolaEncendida ? 0.10f : 0.0f;
        float diffuseFarola = farolaEncendida ? 2.20f : 0.0f;

        spotLights[2] = SpotLight(
            1.0f, 0.93f, 0.72f, ambientFarola, diffuseFarola,
            focoDerWorld.x, focoDerWorld.y, focoDerWorld.z,
            dirFocoDerWorld.x, dirFocoDerWorld.y, dirFocoDerWorld.z,
            1.0f, 0.04f, 0.01f, 40.0f
        );
        spotLights[3] = SpotLight(
            1.0f, 0.93f, 0.72f, ambientFarola, diffuseFarola,
            focoIzqWorld.x, focoIzqWorld.y, focoIzqWorld.z,
            dirFocoIzqWorld.x, dirFocoIzqWorld.y, dirFocoIzqWorld.z,
            1.0f, 0.04f, 0.01f, 40.0f
        );

        // ============================================
 // CONTROLES - CAMARAS HARRY POTTER / HERMIONE
 // ============================================
// Inicializacion una sola vez
        if (!hpCamInit)
        {
            hpCamAereaPos = hpOffsetEscena + glm::vec3(0.0f, hpCamAereaAltura, 18.0f);

            hpCamInteresPos = {
                hpOffsetEscena + glm::vec3(-12.0f, 6.0f, -10.0f),
                hpOffsetEscena + glm::vec3(0.0f, 7.0f,  12.0f),
                hpOffsetEscena + glm::vec3(12.0f, 6.0f, -10.0f)
            };

            hpCamInteresTarget = {
                hpPosGiratiempo + glm::vec3(0.0f, 0.8f, 0.0f),
                hpPosLibro + glm::vec3(0.0f, 0.7f, 0.0f),
                hpPosCopa + glm::vec3(0.0f, 1.0f, 0.0f)
            };

            hpCamInit = true;
        }

        // ============================================
        // CONTROLES - OBJETOS HARRY POTTER
        // ============================================

        // Toggle carro volador -> tecla 1
        bool currAuto1 = keys[GLFW_KEY_1];
        if (currAuto1 && !prevTeclaAuto1) {
            hpCarroAutoActivo = !hpCarroAutoActivo;
        }
        prevTeclaAuto1 = currAuto1;

        // Toggle snitch -> tecla 3
        bool currAuto3 = keys[GLFW_KEY_3];
        if (currAuto3 && !prevTeclaAuto3) {
            hpSnitchAnimacionActiva = !hpSnitchAnimacionActiva;
            hpSnitchVisible = true;
        }
        prevTeclaAuto3 = currAuto3;

        // Actualizacion carro volador
        if (hpCarroAutoActivo)
        {
            if (!hpCarroRuta.empty())
            {
                glm::vec3 target = hpCarroRuta[hpCarroWPActual];
                target.y = hpCarroYFijo;

                glm::vec3 delta = target - hpCarroPos;
                delta.y = 0.0f;
                float dist = glm::length(delta);

                if (dist < 0.60f)
                {
                    hpCarroWPActual = (hpCarroWPActual + 1) % hpCarroRuta.size();

                    target = hpCarroRuta[hpCarroWPActual];
                    target.y = hpCarroYFijo;

                    delta = target - hpCarroPos;
                    delta.y = 0.0f;
                    dist = glm::length(delta);
                }

                if (dist > 0.0001f)
                {
                    glm::vec3 dir = delta / dist;
                    float step = hpVelocidadCarro * deltaTime;
                    if (step > dist) step = dist;

                    hpCarroPos += dir * step;
                    hpCarroPos.y = hpCarroYFijo;

                    hpRotCarro = atan2(dir.x, dir.z) / toRadians;
                }
                else
                {
                    hpCarroPos.y = hpCarroYFijo;
                }
            }

            hpRotLlantas += 250.0f * deltaTime;
            if (hpRotLlantas > 360.0f) hpRotLlantas -= 360.0f;
        }

        // Actualizacion snitch
        if (hpSnitchAnimacionActiva && hpSnitchVisible) {
            UpdatePathFollower(hpPosSnitch, hpRotCarro, hpSnitchWPActual, hpSnitchRuta, hpSnitchVelocidadRuta, deltaTime, 0.80f);
        }

        // ============================================
        // CONTROLES - AVATAR CRASH: Crash
        // ============================================

        // --- Hermione: movimiento con flechas ---
        bool hpMoving = false;
        if (keys[GLFW_KEY_UP]) { hpPosZ -= hpVelocidad * deltaTime; hpRotPersonaje = 180.0f; hpMoving = true; }
        else if (keys[GLFW_KEY_DOWN]) { hpPosZ += hpVelocidad * deltaTime; hpRotPersonaje = 0.0f;   hpMoving = true; }
        else if (keys[GLFW_KEY_LEFT]) { hpPosX -= hpVelocidad * deltaTime; hpRotPersonaje = -90.0f; hpMoving = true; }
        else if (keys[GLFW_KEY_RIGHT]) { hpPosX += hpVelocidad * deltaTime; hpRotPersonaje = 90.0f;  hpMoving = true; }

        if (hpMoving) {
            float hpAnim = sinf(glfwGetTime() * 5.0f) * 30.0f;
            hpRotBrazoDer = hpAnim; hpRotBrazoIzq = -hpAnim;
            hpRotPiernaDer = -hpAnim; hpRotPiernaIzq = hpAnim;
        }
        else {
            hpRotBrazoDer = hpRotBrazoIzq = hpRotPiernaDer = hpRotPiernaIzq = 0.0f;
        }

        // Crash sin control por teclado (queda estatico)
        bool crashMoving = false;
        crashRotBrazoDer = crashRotBrazoIzq = crashRotPiernaDer = crashRotPiernaIzq = 0.0f;

        if (crashMoving) {
            float crashAnim = sin(glfwGetTime() * 5.0f) * 35.0f;
            crashRotBrazoDer = crashAnim; crashRotBrazoIzq = -crashAnim;
            crashRotPiernaDer = -crashAnim; crashRotPiernaIzq = crashAnim;
        }
        else {
            crashRotBrazoDer = crashRotBrazoIzq = crashRotPiernaDer = crashRotPiernaIzq = 0.0f;
        }
        // ============================================
        // CONTROLES - OBJETOS CRASH: Kart
        // ============================================

        // Toggle kart -> tecla 2
        bool currAuto2 = keys[GLFW_KEY_2];
        if (currAuto2 && !prevTeclaAuto2) {
            kartAutoActivo = !kartAutoActivo;
        }
        prevTeclaAuto2 = currAuto2;

        // Actualizacion kart
        if (kartAutoActivo) {
            UpdatePathFollower(kartPos, kartRotY, kartWPActual, kartRuta, kartVelocidad, deltaTime);
            kartRotLlantas += 280.0f * deltaTime;
            if (kartRotLlantas >= 360.0f) kartRotLlantas -= 360.0f;
        }
        // Kart de Crash desactivado por ahora
        // ============================================
        // RENDER
        // ============================================
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Skybox segun hora del dia
        if (factorDia >= 0.5f)
            skyboxDia.DrawSkybox(camera.calculateViewMatrix(), projection);
        else
            skyboxNoche.DrawSkybox(camera.calculateViewMatrix(), projection);

        shaderList[0].UseShader();

        uniformModel = shaderList[0].GetModelLocation();
        uniformProjection = shaderList[0].GetProjectionLocation();
        uniformView = shaderList[0].GetViewLocation();
        uniformEyePosition = shaderList[0].GetEyePositionLocation();
        uniformColor = shaderList[0].getColorLocation();
        uniformTextureOffset = shaderList[0].getOffsetLocation();
        uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
        uniformShininess = shaderList[0].GetShininessLocation();

        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));


        // Uniforms de camara activa (ya configurada arriba con setPosition/lookAt)
        glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(viewMatrix));
        glUniform3f(uniformEyePosition, camPosActiva.x, camPosActiva.y, camPosActiva.z);

        glm::vec3 lowerLightRender = camPosActiva;
        lowerLightRender.y -= 0.3f;
        spotLights[0].SetFlash(lowerLightRender, camDirActiva);

        shaderList[0].SetDirectionalLight(&mainLight);
        shaderList[0].SetPointLights(pointLights, pointLightCount);
        shaderList[0].SetSpotLights(spotLights, spotLightCount);

        glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
        glm::vec2 toffset = glm::vec2(0.0f, 0.0f);
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));

        glm::mat4 model = glm::mat4(1.0f);

        // ============================================
        // RENDER - ESCENARIO
        // ============================================

        // Piso (mesh escalado con textura)
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
        model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        pisoTexture.UseTexture();
        Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
        meshList[0]->RenderMesh();

        // Muralla 1 (lado negativo)
        model = glm::mat4(1.0f);
        model = ApplyTRS(model, posMurallaL_1, rotMurallaL_1, glm::vec3(escMurallaL) * scaleMurallaL);
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
        Pared_M.RenderModel();

        // Muralla 2 (lado positivo, rotada 180)
        model = glm::mat4(1.0f);
        model = ApplyTRS(model, posMurallaL_2, rotMurallaL_2, glm::vec3(escMurallaL) * scaleMurallaL);
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
        Pared_M.RenderModel();

        // Farola: poste (padre jerarquico)
        model = ApplyTRS(baseFarola, offsetPaloFarola, rotPaloFarola, glm::vec3(escFarola) * scalePaloFarola);
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
        PaloFarola_M.RenderModel();

        // Farola: foco derecho (hijo jerarquico del poste)
        model = ApplyTRS(baseFarola, offsetFocoDerFarola, rotFocoDerFarola, glm::vec3(escFarola) * scaleFocoDerFarola);
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
        FocoDer_M.RenderModel();

        // Farola: foco izquierdo (hijo jerarquico del poste)
        model = ApplyTRS(baseFarola, offsetFocoIzqFarola, rotFocoIzqFarola, glm::vec3(escFarola) * scaleFocoIzqFarola);
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
        FocoIzq_M.RenderModel();

        // Banca del parque
        model = glm::mat4(1.0f);
        model = ApplyTRS(model, posBanca, rotBanca, glm::vec3(escBanca) * scaleBanca);
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
        Banca_M.RenderModel();

        // Bloques de pasto (12 instancias en anillo)
        for (int i = 0; i < 12; i++) {
            model = glm::mat4(1.0f);
            model = ApplyTRS(model, pastoPos[i], pastoRot[i], glm::vec3(escBloquePasto) * scaleBloquePasto);
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
            Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
            BloquePasto_M.RenderModel();
        }

        // Reloj de planetas: matriz base comun para todas las piezas
        glm::mat4 baseReloj = glm::mat4(1.0f);
        baseReloj = glm::translate(baseReloj, posReloj);
        baseReloj = glm::rotate(baseReloj, rotReloj.x * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
        baseReloj = glm::rotate(baseReloj, rotReloj.y * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        baseReloj = glm::rotate(baseReloj, rotReloj.z * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));

        // Reloj: base de planetas
        model = ApplyTRS(baseReloj, offsetBasePlanetas, rotBasePlanetas, glm::vec3(escReloj) * scaleBasePlanetas);
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
        BasePlanetas_M.RenderModel();

        // Reloj: cuerpo base
        model = ApplyTRS(baseReloj, offsetRelojBase, rotRelojBase, glm::vec3(escReloj) * scaleRelojBase);
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
        RelojBase_M.RenderModel();

        // Reloj: engrane 1 (orbita Y-, desplazado con offset)
        model = baseReloj;
        model = glm::rotate(model, angOrbitaRelojEngrane1 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, offsetRelojEngrane1);
        model = glm::rotate(model, rotRelojEngrane1.x * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, rotRelojEngrane1.y * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, rotRelojEngrane1.z * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(escReloj) * scaleRelojEngrane1);
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
        RelojEngrane1_M.RenderModel();

        // Reloj: engrane 2 (orbita Y+)
        model = baseReloj;
        model = glm::rotate(model, angOrbitaRelojEngrane2 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, offsetRelojEngrane2);
        model = glm::rotate(model, rotRelojEngrane2.x * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, rotRelojEngrane2.y * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, rotRelojEngrane2.z * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(escReloj) * scaleRelojEngrane2);
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
        RelojEngrane2_M.RenderModel();

        // Reloj: engrane 3 (orbita Y-)
        model = baseReloj;
        model = glm::rotate(model, angOrbitaRelojEngrane3 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, offsetRelojEngrane3);
        model = glm::rotate(model, rotRelojEngrane3.x * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, rotRelojEngrane3.y * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, rotRelojEngrane3.z * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(escReloj) * scaleRelojEngrane3);
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
        RelojEngrane3_M.RenderModel();

        // Reloj: engrane 4 (orbita Y+)
        model = baseReloj;
        model = glm::rotate(model, angOrbitaRelojEngrane4 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, offsetRelojEngrane4);
        model = glm::rotate(model, rotRelojEngrane4.x * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, rotRelojEngrane4.y * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, rotRelojEngrane4.z * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(escReloj) * scaleRelojEngrane4);
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
        RelojEngrane4_M.RenderModel();

        // Reloj: galaxia/red (orbita Y-)
        model = baseReloj;
        model = glm::rotate(model, angOrbitaRelojGalRedE * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, offsetRelojGalRedE);
        model = glm::rotate(model, rotRelojGalRedE.x * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, rotRelojGalRedE.y * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, rotRelojGalRedE.z * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(escReloj) * scaleRelojGalRedE);
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
        RelojGalRedE_M.RenderModel();

        // Reloj: planetas (orbita Y-)
        model = baseReloj;
        model = glm::rotate(model, angOrbitaRelojPlanetas * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, offsetRelojPlanetas);
        model = glm::rotate(model, rotRelojPlanetas.x * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, rotRelojPlanetas.y * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, rotRelojPlanetas.z * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(escReloj) * scaleRelojPlanetas);
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
        RelojPlanetas_M.RenderModel();

        // ============================================
        // RENDER - AVATAR BIOSHOCK: Big Daddy
        // ============================================

        // Matriz base del Big Daddy padre jerarquico posicion + direccion real de la ruta
        glm::mat4 modelaux_BD = glm::mat4(1.0f);
        modelaux_BD = glm::translate(modelaux_BD, bigDaddyPosActual);
        modelaux_BD = glm::rotate(modelaux_BD, bigDaddyRotY * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));

        // Big Daddy: torso (hijo del padre)
        model = modelaux_BD;
        model = glm::translate(model, offsetCuerpo);
        model = glm::rotate(model, rotCuerpo.x * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, rotCuerpo.y * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, rotCuerpo.z * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(escBigDaddy));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
        BigDaddyCuerpo_M.RenderModel();

        // Big Daddy: brazo izquierdo (animado en caminata)
        model = modelaux_BD;
        model = glm::translate(model, offsetBrazoIzq);
        model = glm::rotate(model, (rotBrazoIzq.x + anguloBrazoIzq) * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, rotBrazoIzq.y * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, rotBrazoIzq.z * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(escBigDaddy));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
        BigDaddyBrazoIzq_M.RenderModel();

        // Big Daddy: brazo derecho (animado en caminata)
        model = modelaux_BD;
        model = glm::translate(model, offsetBrazoDer);
        model = glm::rotate(model, (rotBrazoDer.x + anguloBrazoDer) * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, rotBrazoDer.y * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, rotBrazoDer.z * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(escBigDaddy));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
        BigDaddyBrazoDer_M.RenderModel();

        // Big Daddy: pierna izquierda (animada en caminata)
        model = modelaux_BD;
        model = glm::translate(model, offsetPiernaIzq);
        model = glm::rotate(model, (rotPiernaIzq.x + anguloPiernaIzq) * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, rotPiernaIzq.y * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, rotPiernaIzq.z * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(escBigDaddy));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
        BigDaddyPiernaIzq_M.RenderModel();

        // Big Daddy: pierna derecha (animada en caminata)
        model = modelaux_BD;
        model = glm::translate(model, offsetPiernaDer);
        model = glm::rotate(model, (rotPiernaDer.x + anguloPiernaDer) * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, rotPiernaDer.y * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, rotPiernaDer.z * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(escBigDaddy));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
        BigDaddyPiernaDer_M.RenderModel();

        // Big Daddy: taladro (hijo del brazo der, escala -X para espejear el mesh)
        model = modelaux_BD;
        model = glm::translate(model, offsetTaladro * escBigDaddy);
        model = glm::rotate(model, -anguloBrazoDer * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, rotTaladro.z * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::rotate(model, taladroRot * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(-escBigDaddy, escBigDaddy, escBigDaddy));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
        glCullFace(GL_FRONT);
        BigDaddyTaladro_M.RenderModel();
        glCullFace(GL_BACK);

        // ============================================
        // RENDER - OBJETOS BIOSHOCK
        // ============================================

        // Medkit: matriz padre comun para las tres piezas
        glm::mat4 modelaux_MK = glm::mat4(1.0f);
        modelaux_MK = glm::translate(modelaux_MK, posMedkit);

        // Medkit: cuerpo central (escala -X para espejear)
        model = modelaux_MK;
        model = glm::scale(model, glm::vec3(-escMedkit, escMedkit, escMedkit));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
        glCullFace(GL_FRONT);
        BolsaMid_M.RenderModel();
        glCullFace(GL_BACK);

        // Medkit: tapa derecha (se abre al acercarse, rotacion +puertaAngulo en X)
        model = modelaux_MK;
        model = glm::translate(model, offsetBolsaDer);
        model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, puertaAngulo * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(-escMedkit, escMedkit, escMedkit));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
        glCullFace(GL_FRONT);
        BolsaDer_M.RenderModel();
        glCullFace(GL_BACK);

        // Medkit: tapa izquierda (se abre al acercarse, rotacion -puertaAngulo en X)
        model = modelaux_MK;
        model = glm::translate(model, offsetBolsaIzq);
        model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, -puertaAngulo * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(-escMedkit, escMedkit, escMedkit));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
        glCullFace(GL_FRONT);
        BolsaIzq_M.RenderModel();
        glCullFace(GL_BACK);

        // Plasmido: flota, rota en Y y pulsa en escala (latido)
        glm::mat4 modelaux_PL = glm::mat4(1.0f);
        modelaux_PL = glm::translate(modelaux_PL, posPlasmido + glm::vec3(0.0f, plasOffsetY, 0.0f));
        modelaux_PL = glm::rotate(modelaux_PL, plasRotY * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        model = modelaux_PL;
        model = glm::scale(model, glm::vec3(plasEscActual));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
        Plasmido_M.RenderModel();

        // ============================================
        // RENDER - AVATAR HARRY POTTER: Hermione
        // ============================================
        glm::mat4 modelaux;

        // Hermione: cuerpo completo (padre jerarquico)
        model = glm::mat4(1.0f);
        model = glm::translate(model, hpOffsetEscena + glm::vec3(hpPosX, 1.0f, hpPosZ));
        model = glm::rotate(model, hpRotPersonaje * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(1.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
        Hermione_HP_M.RenderModel();

        // Hermione: brazo derecho (hijo del cuerpo)
        modelaux = model;
        modelaux = glm::translate(modelaux, glm::vec3(0.5f, 0.8f, 0.0f));
        modelaux = glm::rotate(modelaux, hpRotBrazoDer * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
        Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
        BrazoDer_HP_M.RenderModel();

        // Hermione: brazo izquierdo (hijo del cuerpo)
        modelaux = model;
        modelaux = glm::translate(modelaux, glm::vec3(-0.5f, 0.8f, 0.0f));
        modelaux = glm::rotate(modelaux, hpRotBrazoIzq * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
        Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
        BrazoIzq_HP_M.RenderModel();
        // Hermione: pierna derecha (hija del cuerpo)
        modelaux = model;
        modelaux = glm::translate(modelaux, glm::vec3(0.1f, -0.7f, 0.0f));
        modelaux = glm::rotate(modelaux, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); // giro fijo
        modelaux = glm::rotate(modelaux, hpRotPiernaDer * toRadians, glm::vec3(1.0f, 0.0f, 0.0f)); // animación
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
        Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
        PiernaDer_HP_M.RenderModel();

        // Hermione: pierna izquierda (hija del cuerpo)
        modelaux = model;
        modelaux = glm::translate(modelaux, glm::vec3(-0.1f, -0.7f, 0.0f));
        modelaux = glm::rotate(modelaux, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); // giro fijo
        modelaux = glm::rotate(modelaux, hpRotPiernaIzq * toRadians, glm::vec3(1.0f, 0.0f, 0.0f)); // animación
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
        Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
        PiernaIzq_HP_M.RenderModel();

        // ============================================
        // RENDER - OBJETOS HARRY POTTER
        // ============================================
// Carro volador: padre jerarquico comun para carro + 4 llantas
        glm::mat4 hpCarroBase = glm::mat4(1.0f);
        hpCarroBase = glm::translate(hpCarroBase, hpCarroPos + glm::vec3(0.0f, -2.0f, 0.0f));
        hpCarroBase = glm::rotate(hpCarroBase, hpRotCarro * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        hpCarroBase = glm::scale(hpCarroBase, glm::vec3(2.0f));

        // Carro carroceria
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(hpCarroBase));
        Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
        Carro_HP_M.RenderModel();

        // Carro llanta frontal derecha
        model = hpCarroBase;
        model = glm::translate(model, glm::vec3(0.52f, -0.01f, 0.456f));
        model = glm::rotate(model, hpRotLlantas * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        LlantaFD_HP_M.RenderModel();

        // Carro llanta frontal izquierda
        model = hpCarroBase;
        model = glm::translate(model, glm::vec3(-0.52f, -0.01f, 0.465f));
        model = glm::rotate(model, hpRotLlantas * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        LlantaFI_HP_M.RenderModel();

        // Carro llanta trasera derecha
        model = hpCarroBase;
        model = glm::translate(model, glm::vec3(0.52f, -0.01f, -0.465f));
        model = glm::rotate(model, hpRotLlantas * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        LlantaAD_HP_M.RenderModel();

        // Carro llanta trasera izquierda
        model = hpCarroBase;
        model = glm::translate(model, glm::vec3(-0.52f, -0.01f, -0.465f));
        model = glm::rotate(model, hpRotLlantas * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        LlantaAI_HP_M.RenderModel();

        // Bolsa de Hermione (posicion fija en la escena)
        model = glm::mat4(1.0f);
        model = glm::translate(model, hpPosBolsa);
        model = glm::scale(model, glm::vec3(1.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
        Bolsa_HP_M.RenderModel();

        // Giratiempo (escala reducida a 0.5)
        model = glm::mat4(1.0f);
        model = glm::translate(model, hpPosGiratiempo);
        model = glm::scale(model, glm::vec3(0.5f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
        Giratiempo_HP_M.RenderModel();

        // Libro de hechizos
        model = glm::mat4(1.0f);
        model = glm::translate(model, hpPosLibro);
        model = glm::scale(model, glm::vec3(1.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
        LibroHechizos_HP_M.RenderModel();

        // Snitch dorada: solo si es visible (animacion de vuelo senoidal)
        if (hpSnitchVisible)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, hpPosSnitch);
            model = glm::scale(model, glm::vec3(2.4f));
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
            Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
            Snitch_HP_M.RenderModel();
        }

        // Copa de los tres magos
        model = glm::mat4(1.0f);
        model = glm::translate(model, hpPosCopa + glm::vec3(0.0f, copaOffsetY, 0.0f));
        model = glm::rotate(model, copaRotY * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(copaEscActual));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
        Copa_HP_M.RenderModel();

        // Expreso de Hogwarts: cabina
        model = glm::mat4(1.0f);
        model = glm::translate(model, posCabinaExpreso);
        model = glm::scale(model, glm::vec3(1.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
        CabinaExpreso_M.RenderModel();

        // Expreso: vagon
        model = glm::mat4(1.0f);
        model = glm::translate(model, posVagonExpreso);
        model = glm::scale(model, glm::vec3(1.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
        VagonExpreso_M.RenderModel();

        // Expreso: biela izquierda
        model = glm::mat4(1.0f);
        model = glm::translate(model, posBielaIzqExpreso);
        model = glm::scale(model, glm::vec3(1.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
        BielaIzqExpreso_M.RenderModel();

        // Expreso: biela motriz izquierda
        model = glm::mat4(1.0f);
        model = glm::translate(model, posBielaMotrizIzq);
        model = glm::scale(model, glm::vec3(1.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
        BielaMotrizIzqExpreso_M.RenderModel();

        // Expreso: eje delantero
        model = glm::mat4(1.0f);
        model = glm::translate(model, posEjeDelanExpreso);
        model = glm::scale(model, glm::vec3(1.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
        EjeDelanExpreso_M.RenderModel();

        // Expreso: eje trasero
        model = glm::mat4(1.0f);
        model = glm::translate(model, posEjeTrasExpreso);
        model = glm::scale(model, glm::vec3(1.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
        EjeTrasExpreso_M.RenderModel();

        // Expreso: eje del vagon
        model = glm::mat4(1.0f);
        model = glm::translate(model, posEjeVagonExpreso);
        model = glm::scale(model, glm::vec3(1.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
        EjeVagonExpreso_M.RenderModel();

        // Expreso: piston izquierdo
        model = glm::mat4(1.0f);
        model = glm::translate(model, posPistonIzqExpreso);
        model = glm::scale(model, glm::vec3(1.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
        PistonIzqExpreso_M.RenderModel();

        // Expreso: llanta del steam car 3
        model = glm::mat4(1.0f);
        model = glm::translate(model, posLlantaSteamCar3);
        model = glm::scale(model, glm::vec3(1.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
        LlantaSteamCar3_M.RenderModel();

        // Expreso: bicicleta/steam car 3 de Harry
        model = glm::mat4(1.0f);
        model = glm::translate(model, posSteamCar3);
        model = glm::scale(model, glm::vec3(1.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
        SteamCar3_M.RenderModel();

        // ============================================
        // RENDER - AVATAR CRASH: Crash
        // ============================================

        // Crash: cuerpo (padre jerarquico)
        model = glm::mat4(1.0f);
        model = glm::translate(model, crashOffsetEscena + glm::vec3(crashPosX, 1.0f, crashPosZ));
        model = glm::rotate(model, crashRotY * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(1.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
        Crash_Cuerpo_M.RenderModel();

        // Crash: brazo derecho (animado)
        model = glm::mat4(1.0f);
        model = glm::translate(model, crashOffsetEscena + glm::vec3(crashPosX, 1.0f, crashPosZ));
        model = glm::rotate(model, crashRotY * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, glm::vec3(0.45f, 0.75f, 0.0f));
        model = glm::rotate(model, crashRotBrazoDer * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Crash_BrazoDer_M.RenderModel();

        // Crash: brazo izquierdo (animado)
        model = glm::mat4(1.0f);
        model = glm::translate(model, crashOffsetEscena + glm::vec3(crashPosX, 1.0f, crashPosZ));
        model = glm::rotate(model, crashRotY * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, glm::vec3(-0.45f, 0.75f, 0.0f));
        model = glm::rotate(model, crashRotBrazoIzq * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Crash_BrazoIzq_M.RenderModel();

        // Crash: pierna derecha (animada)
        model = glm::mat4(1.0f);
        model = glm::translate(model, crashOffsetEscena + glm::vec3(crashPosX, 1.0f, crashPosZ));
        model = glm::rotate(model, crashRotY * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, glm::vec3(0.15f, -0.7f, 0.0f));
        model = glm::rotate(model, crashRotPiernaDer * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Crash_PiernaDer_M.RenderModel();

        // Crash: pierna izquierda (animada)
        model = glm::mat4(1.0f);
        model = glm::translate(model, crashOffsetEscena + glm::vec3(crashPosX, 1.0f, crashPosZ));
        model = glm::rotate(model, crashRotY * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, glm::vec3(-0.15f, -0.7f, 0.0f));
        model = glm::rotate(model, crashRotPiernaIzq * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Crash_PiernaIzq_M.RenderModel();

        // ============================================
        // RENDER - OBJETOS CRASH
        // ============================================

// Go-kart padre jerarquico comun para kart + 4 llantas
        glm::mat4 kartBase = glm::mat4(1.0f);
        kartBase = glm::translate(kartBase, kartPos + glm::vec3(0.0f, -2.0f, 0.0f));
        kartBase = glm::rotate(kartBase, (kartRotY - 90.0f) * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        kartBase = glm::scale(kartBase, glm::vec3(2.0f));

        // Kart carroceria
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(kartBase));
        Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
        Crash_GoKart_M.RenderModel();

        // Kart llanta frontal derecha
        model = kartBase;
        model = glm::translate(model, glm::vec3(0.52f, -0.01f, 0.456f));
        model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, kartRotLlantas * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        LlantaFD_Crash_M.RenderModel();

        // Kart llanta frontal izquierda
        model = kartBase;
        model = glm::translate(model, glm::vec3(-0.52f, -0.01f, 0.456f));
        model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, kartRotLlantas * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        LlantaFI_Crash_M.RenderModel();

        // Kart llanta trasera derecha
        model = kartBase;
        model = glm::translate(model, glm::vec3(0.52f, -0.01f, -0.456f));
        model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, kartRotLlantas * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        LlantaAD_Crash_M.RenderModel();

        // Kart llanta trasera izquierda
        model = kartBase;
        model = glm::translate(model, glm::vec3(-0.52f, -0.01f, -0.456f));
        model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, kartRotLlantas * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        LlantaAI_Crash_M.RenderModel();

        // Steam car de Crash (decorativo, posicion fija)
        model = glm::mat4(1.0f);
        model = glm::translate(model, posSteamCar);
        model = glm::scale(model, glm::vec3(1.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
        SteamCar_Crash_M.RenderModel();

        // Props del universo Crash (linea de display en Z+15 relativo a crashOffsetEscena)
        model = glm::mat4(1.0f);
        model = glm::translate(model, posArbolWumpa);
        model = glm::scale(model, glm::vec3(1.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
        ArbolWumpa_M.RenderModel();

        model = glm::mat4(1.0f);
        model = glm::translate(model, posBaseCilindro);
        model = glm::scale(model, glm::vec3(1.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
        BaseCilindro_M.RenderModel();

        model = glm::mat4(1.0f);
        model = glm::translate(model, posBolaRoca);
        model = glm::scale(model, glm::vec3(1.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
        BolaRoca_M.RenderModel();

        model = glm::mat4(1.0f);
        model = glm::translate(model, posCajaAku);
        model = glm::scale(model, glm::vec3(1.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
        CajaAku_M.RenderModel();

        model = glm::mat4(1.0f);
        model = glm::translate(model, posCajaCaraCrash);
        model = glm::scale(model, glm::vec3(1.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
        CajaCaraCrash_M.RenderModel();

        model = glm::mat4(1.0f);
        model = glm::translate(model, posCajaCheckPoint);
        model = glm::scale(model, glm::vec3(1.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
        CajaCheckPoint_M.RenderModel();

        model = glm::mat4(1.0f);
        model = glm::translate(model, posCajaFlechaArriba);
        model = glm::scale(model, glm::vec3(1.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
        CajaFlechaArriba_M.RenderModel();

        model = glm::mat4(1.0f);
        model = glm::translate(model, posCajaMetalExcl);
        model = glm::scale(model, glm::vec3(1.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
        CajaMetalExclamation_M.RenderModel();

        model = glm::mat4(1.0f);
        model = glm::translate(model, posCajaNitro);
        model = glm::scale(model, glm::vec3(1.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
        CajaNitro_M.RenderModel();

        model = glm::mat4(1.0f);
        model = glm::translate(model, posCajaNitroExcl);
        model = glm::scale(model, glm::vec3(1.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
        CajaNitroExclamation_M.RenderModel();

        model = glm::mat4(1.0f);
        model = glm::translate(model, posCajaNormal);
        model = glm::scale(model, glm::vec3(1.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
        CajaNormalCrash_M.RenderModel();

        model = glm::mat4(1.0f);
        model = glm::translate(model, posCajaQuestion);
        model = glm::scale(model, glm::vec3(1.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
        CajaQuestion_M.RenderModel();

        model = glm::mat4(1.0f);
        model = glm::translate(model, posCajaTNT);
        model = glm::scale(model, glm::vec3(1.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
        CajaTNT_M.RenderModel();

        model = glm::mat4(1.0f);
        model = glm::translate(model, posCocoOnGoKart);
        model = glm::scale(model, glm::vec3(1.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
        CocoOnGoKart_M.RenderModel();

        model = glm::mat4(1.0f);
        model = glm::translate(model, posCortexOnGoKart);
        model = glm::scale(model, glm::vec3(1.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
        CortexOnGoKart_M.RenderModel();

        model = glm::mat4(1.0f);
        model = glm::translate(model, posCrystalCrash);
        model = glm::scale(model, glm::vec3(1.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
        CrystalCrash_M.RenderModel();

        model = glm::mat4(1.0f);
        model = glm::translate(model, posCrystalHielo);
        model = glm::scale(model, glm::vec3(1.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
        CrystalHielo_M.RenderModel();

        model = glm::mat4(1.0f);
        model = glm::translate(model, posFrutaWumpa);
        model = glm::scale(model, glm::vec3(1.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
        FrutaWumpa_M.RenderModel();

        model = glm::mat4(1.0f);
        model = glm::translate(model, posJoyasCrash);
        model = glm::scale(model, glm::vec3(1.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
        JoyasCrash_M.RenderModel();

        model = glm::mat4(1.0f);
        model = glm::translate(model, posMascaraAku);
        model = glm::scale(model, glm::vec3(1.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
        MascaraAku_M.RenderModel();

        model = glm::mat4(1.0f);
        model = glm::translate(model, posMascaraUka);
        model = glm::scale(model, glm::vec3(1.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
        MascaraUka_M.RenderModel();

        model = glm::mat4(1.0f);
        model = glm::translate(model, posPista);
        model = glm::scale(model, glm::vec3(3.0f, 1.0f, 4.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
        Pista_M.RenderModel();

        model = glm::mat4(1.0f);
        model = glm::translate(model, posSteamCar1);
        model = glm::scale(model, glm::vec3(1.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
        SteamCar1_M.RenderModel();

        model = glm::mat4(1.0f);
        model = glm::translate(model, posSteamCar2);
        model = glm::scale(model, glm::vec3(1.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
        SteamCar2_M.RenderModel();

        model = glm::mat4(1.0f);
        model = glm::translate(model, posTotem1Crash);
        model = glm::scale(model, glm::vec3(1.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
        Totem1Crash_M.RenderModel();

        model = glm::mat4(1.0f);
        model = glm::translate(model, posTotem2Crash);
        model = glm::scale(model, glm::vec3(1.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
        Totem2Crash_M.RenderModel();

        model = glm::mat4(1.0f);
        model = glm::translate(model, posRuedaSteamCar2);
        model = glm::scale(model, glm::vec3(1.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
        RuedaSteamCar2_M.RenderModel();

        // ============================================
        // FIN DEL FRAME
        // ============================================
        glUseProgram(0);
        mainWindow.swapBuffers();
    }

    return 0;
}