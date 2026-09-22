using System.Collections.Generic;
using UnityEngine;

public class TPRealidadVirtual : MonoBehaviour
{
    Transform raizGema;
    Transform planosPivote;
    Material materialPlanoA;
    Material materialPlanoB;

    Vector3 ejeRotacion = Vector3.up;
    float velocidadBase = 45f;
    float velocidadActual;
    int contadorPatron;

    void Start()
    {
        velocidadActual = velocidadBase;

        Shader shaderSuperficie = BuscarShaderSuperficie();

        ConfigurarCamara();
        ConfigurarLuces();
        ConstruirGema(shaderSuperficie);
        ConstruirPlanos(shaderSuperficie);
        AplicarPatron();
    }

    void Update()
    {
        if (Input.GetKeyDown(KeyCode.UpArrow))
        {
            ejeRotacion = Vector3.right;
            velocidadActual = velocidadBase;
        }
        else if (Input.GetKeyDown(KeyCode.DownArrow))
        {
            ejeRotacion = Vector3.right;
            velocidadActual = -velocidadBase;
        }
        else if (Input.GetKeyDown(KeyCode.LeftArrow))
        {
            ejeRotacion = Vector3.up;
            velocidadActual = velocidadBase;
        }
        else if (Input.GetKeyDown(KeyCode.RightArrow))
        {
            ejeRotacion = Vector3.up;
            velocidadActual = -velocidadBase;
        }
        else if (Input.GetKeyDown(KeyCode.D))
        {
            velocidadActual = 0f;
        }
        else if (Input.GetKeyDown(KeyCode.R))
        {
            Reiniciar();
        }

        if (velocidadActual != 0f)
        {
            planosPivote.Rotate(ejeRotacion, velocidadActual * Time.deltaTime, Space.Self);
        }

        raizGema.Rotate(Vector3.up, 9f * Time.deltaTime, Space.Self);
    }

    void OnGUI()
    {
        GUI.color = Color.white;
        GUI.Label(new Rect(16, 16, 620, 24), "Flechas: eje/sentido de rotacion | D: detener | R: reiniciar (cambia el patron)");
    }

    void Reiniciar()
    {
        planosPivote.localRotation = Quaternion.identity;
        ejeRotacion = Vector3.up;
        velocidadActual = velocidadBase;
        contadorPatron++;
        AplicarPatron();
    }

    void ConfigurarCamara()
    {
        GameObject objetoCamara = new GameObject("Camara");
        Camera camara = objetoCamara.AddComponent<Camera>();
        objetoCamara.tag = "MainCamera";
        camara.clearFlags = CameraClearFlags.SolidColor;
        camara.backgroundColor = Color.black;
        objetoCamara.transform.position = new Vector3(0f, 3.2f, 9.5f);
        objetoCamara.transform.LookAt(Vector3.zero, Vector3.up);
    }

    void ConfigurarLuces()
    {
        GameObject objetoLuzClave = new GameObject("LuzClave");
        Light luzClave = objetoLuzClave.AddComponent<Light>();
        luzClave.type = LightType.Directional;
        luzClave.intensity = 1.1f;
        objetoLuzClave.transform.rotation = Quaternion.Euler(45f, -35f, 0f);

        GameObject objetoLuzRelleno = new GameObject("LuzRelleno");
        Light luzRelleno = objetoLuzRelleno.AddComponent<Light>();
        luzRelleno.type = LightType.Point;
        luzRelleno.intensity = 3f;
        luzRelleno.range = 14f;
        luzRelleno.color = new Color(0.35f, 0.65f, 1f);
        objetoLuzRelleno.transform.position = new Vector3(-4f, 1.5f, 4f);
    }

    void ConstruirGema(Shader shaderSuperficie)
    {
        GameObject raiz = new GameObject("Gema");
        raizGema = raiz.transform;
        raizGema.SetParent(transform, false);

        Material materialGema = new Material(shaderSuperficie);
        AplicarColor(materialGema, new Color(0.15f, 0.85f, 0.9f));
        AplicarBrillo(materialGema, 0.65f, 0.85f);

        GameObject objetoSuperior = new GameObject("ConoSuperior");
        objetoSuperior.transform.SetParent(raizGema, false);
        objetoSuperior.transform.localPosition = new Vector3(0f, 0.7f, 0f);
        MeshFilter filtroSuperior = objetoSuperior.AddComponent<MeshFilter>();
        filtroSuperior.mesh = CrearMallaCono(1.1f, 1.4f, 24, true);
        MeshRenderer rendererSuperior = objetoSuperior.AddComponent<MeshRenderer>();
        rendererSuperior.material = materialGema;

        GameObject objetoInferior = new GameObject("ConoInferior");
        objetoInferior.transform.SetParent(raizGema, false);
        objetoInferior.transform.localPosition = new Vector3(0f, -0.7f, 0f);
        MeshFilter filtroInferior = objetoInferior.AddComponent<MeshFilter>();
        filtroInferior.mesh = CrearMallaCono(1.1f, 1.4f, 24, false);
        MeshRenderer rendererInferior = objetoInferior.AddComponent<MeshRenderer>();
        rendererInferior.material = materialGema;
    }

    void ConstruirPlanos(Shader shaderSuperficie)
    {
        GameObject raiz = new GameObject("PlanosPivote");
        planosPivote = raiz.transform;
        planosPivote.SetParent(transform, false);

        Mesh mallaPlano = CrearMallaPlano(7f, 2.2f);

        materialPlanoA = new Material(shaderSuperficie);
        GameObject objetoA = new GameObject("PlanoA");
        objetoA.transform.SetParent(planosPivote, false);
        objetoA.transform.localRotation = Quaternion.Euler(90f, 0f, 0f);
        MeshFilter filtroA = objetoA.AddComponent<MeshFilter>();
        filtroA.mesh = mallaPlano;
        MeshRenderer rendererA = objetoA.AddComponent<MeshRenderer>();
        rendererA.material = materialPlanoA;

        materialPlanoB = new Material(shaderSuperficie);
        GameObject objetoB = new GameObject("PlanoB");
        objetoB.transform.SetParent(planosPivote, false);
        objetoB.transform.localRotation = Quaternion.Euler(90f, 65f, 0f);
        MeshFilter filtroB = objetoB.AddComponent<MeshFilter>();
        filtroB.mesh = mallaPlano;
        MeshRenderer rendererB = objetoB.AddComponent<MeshRenderer>();
        rendererB.material = materialPlanoB;
    }

    void AplicarPatron()
    {
        Color[][] paletas = new Color[][]
        {
            new Color[] { new Color(1f, 0.45f, 0f), new Color(0.15f, 0.85f, 0.2f) },
            new Color[] { new Color(0.85f, 0.1f, 0.15f), new Color(0.95f, 0.85f, 0.2f) },
            new Color[] { new Color(0.1f, 0.55f, 0.95f), new Color(0.9f, 0.9f, 0.95f) }
        };

        int tipo = contadorPatron % 3;
        Color[] paleta = paletas[(contadorPatron / 3) % paletas.Length];

        Texture2D texturaA;
        Texture2D texturaB;

        if (tipo == 0)
        {
            texturaA = CrearTexturaAjedrez(paleta[0], paleta[1], 8, 128);
            texturaB = CrearTexturaAjedrez(paleta[1], paleta[0], 8, 128);
        }
        else if (tipo == 1)
        {
            texturaA = CrearTexturaFranjas(paleta[0], paleta[1], 10, 128);
            texturaB = CrearTexturaFranjas(paleta[1], paleta[0], 10, 128);
        }
        else
        {
            texturaA = CrearTexturaAnillos(paleta[0], paleta[1], 6, 128);
            texturaB = CrearTexturaAnillos(paleta[1], paleta[0], 6, 128);
        }

        AplicarTextura(materialPlanoA, texturaA);
        AplicarTextura(materialPlanoB, texturaB);
    }

    static Shader BuscarShaderSuperficie()
    {
        string[] nombres = new string[]
        {
            "Universal Render Pipeline/Lit",
            "Standard",
            "Legacy Shaders/Diffuse",
            "Unlit/Texture",
            "Sprites/Default"
        };

        for (int i = 0; i < nombres.Length; i++)
        {
            Shader encontrado = Shader.Find(nombres[i]);
            if (encontrado != null)
            {
                return encontrado;
            }
        }

        return null;
    }

    static void AplicarColor(Material material, Color color)
    {
        if (material.HasProperty("_BaseColor"))
        {
            material.SetColor("_BaseColor", color);
        }
        if (material.HasProperty("_Color"))
        {
            material.SetColor("_Color", color);
        }
    }

    static void AplicarTextura(Material material, Texture2D textura)
    {
        if (material.HasProperty("_BaseMap"))
        {
            material.SetTexture("_BaseMap", textura);
        }
        if (material.HasProperty("_MainTex"))
        {
            material.SetTexture("_MainTex", textura);
        }
    }

    static void AplicarBrillo(Material material, float metalico, float suavidad)
    {
        if (material.HasProperty("_Metallic"))
        {
            material.SetFloat("_Metallic", metalico);
        }
        if (material.HasProperty("_Smoothness"))
        {
            material.SetFloat("_Smoothness", suavidad);
        }
        if (material.HasProperty("_Glossiness"))
        {
            material.SetFloat("_Glossiness", suavidad);
        }
    }

    static void AgregarTrianguloDobleCara(List<int> triangulos, int a, int b, int c)
    {
        triangulos.Add(a);
        triangulos.Add(b);
        triangulos.Add(c);
        triangulos.Add(a);
        triangulos.Add(c);
        triangulos.Add(b);
    }

    static Mesh CrearMallaCono(float radio, float altura, int segmentos, bool apiceArriba)
    {
        List<Vector3> vertices = new List<Vector3>();
        List<Vector3> normales = new List<Vector3>();
        List<int> triangulos = new List<int>();

        float ySuperior = apiceArriba ? altura / 2f : -altura / 2f;
        float yInferior = apiceArriba ? -altura / 2f : altura / 2f;
        Vector3 apice = new Vector3(0f, ySuperior, 0f);
        float pendiente = radio / altura;

        for (int i = 0; i < segmentos; i++)
        {
            float anguloA = (float)i / segmentos * Mathf.PI * 2f;
            float anguloB = (float)(i + 1) / segmentos * Mathf.PI * 2f;

            Vector3 baseA = new Vector3(Mathf.Cos(anguloA) * radio, yInferior, Mathf.Sin(anguloA) * radio);
            Vector3 baseB = new Vector3(Mathf.Cos(anguloB) * radio, yInferior, Mathf.Sin(anguloB) * radio);

            Vector3 normalA = new Vector3(Mathf.Cos(anguloA), pendiente, Mathf.Sin(anguloA)).normalized;
            Vector3 normalB = new Vector3(Mathf.Cos(anguloB), pendiente, Mathf.Sin(anguloB)).normalized;

            int indiceApice = vertices.Count;
            vertices.Add(apice);
            normales.Add(((normalA + normalB) * 0.5f).normalized);

            int indiceA = vertices.Count;
            vertices.Add(baseA);
            normales.Add(normalA);

            int indiceB = vertices.Count;
            vertices.Add(baseB);
            normales.Add(normalB);

            AgregarTrianguloDobleCara(triangulos, indiceApice, indiceA, indiceB);
        }

        int indiceCentro = vertices.Count;
        vertices.Add(new Vector3(0f, yInferior, 0f));
        normales.Add(new Vector3(0f, apiceArriba ? -1f : 1f, 0f));

        for (int i = 0; i < segmentos; i++)
        {
            float anguloA = (float)i / segmentos * Mathf.PI * 2f;
            float anguloB = (float)(i + 1) / segmentos * Mathf.PI * 2f;

            Vector3 baseA = new Vector3(Mathf.Cos(anguloA) * radio, yInferior, Mathf.Sin(anguloA) * radio);
            Vector3 baseB = new Vector3(Mathf.Cos(anguloB) * radio, yInferior, Mathf.Sin(anguloB) * radio);
            Vector3 normalCentro = new Vector3(0f, apiceArriba ? -1f : 1f, 0f);

            int indiceA = vertices.Count;
            vertices.Add(baseA);
            normales.Add(normalCentro);

            int indiceB = vertices.Count;
            vertices.Add(baseB);
            normales.Add(normalCentro);

            AgregarTrianguloDobleCara(triangulos, indiceCentro, indiceA, indiceB);
        }

        Mesh malla = new Mesh();
        malla.SetVertices(vertices);
        malla.SetNormals(normales);
        malla.SetTriangles(triangulos, 0);
        malla.RecalculateBounds();
        return malla;
    }

    static Mesh CrearMallaPlano(float ancho, float profundidad)
    {
        float mitadAncho = ancho / 2f;
        float mitadProfundidad = profundidad / 2f;

        Vector3[] vertices = new Vector3[]
        {
            new Vector3(-mitadAncho, 0f, -mitadProfundidad),
            new Vector3(mitadAncho, 0f, -mitadProfundidad),
            new Vector3(mitadAncho, 0f, mitadProfundidad),
            new Vector3(-mitadAncho, 0f, mitadProfundidad)
        };

        Vector3[] normales = new Vector3[] { Vector3.up, Vector3.up, Vector3.up, Vector3.up };

        Vector2[] uvs = new Vector2[]
        {
            new Vector2(0f, 0f),
            new Vector2(1f, 0f),
            new Vector2(1f, 1f),
            new Vector2(0f, 1f)
        };

        List<int> triangulos = new List<int>();
        AgregarTrianguloDobleCara(triangulos, 0, 1, 2);
        AgregarTrianguloDobleCara(triangulos, 0, 2, 3);

        Mesh malla = new Mesh();
        malla.SetVertices(vertices);
        malla.SetNormals(normales);
        malla.SetUVs(0, uvs);
        malla.SetTriangles(triangulos, 0);
        malla.RecalculateBounds();
        return malla;
    }

    static Texture2D CrearTexturaAjedrez(Color colorA, Color colorB, int celdas, int resolucion)
    {
        Texture2D textura = new Texture2D(resolucion, resolucion, TextureFormat.RGBA32, false);
        textura.filterMode = FilterMode.Point;
        textura.wrapMode = TextureWrapMode.Clamp;

        int tamCelda = Mathf.Max(1, resolucion / celdas);
        Color[] pixeles = new Color[resolucion * resolucion];

        for (int y = 0; y < resolucion; y++)
        {
            for (int x = 0; x < resolucion; x++)
            {
                int cx = x / tamCelda;
                int cy = y / tamCelda;
                pixeles[y * resolucion + x] = ((cx + cy) % 2 == 0) ? colorA : colorB;
            }
        }

        textura.SetPixels(pixeles);
        textura.Apply();
        return textura;
    }

    static Texture2D CrearTexturaFranjas(Color colorA, Color colorB, int franjas, int resolucion)
    {
        Texture2D textura = new Texture2D(resolucion, resolucion, TextureFormat.RGBA32, false);
        textura.filterMode = FilterMode.Point;
        textura.wrapMode = TextureWrapMode.Clamp;

        int anchoBanda = Mathf.Max(1, resolucion / franjas);
        Color[] pixeles = new Color[resolucion * resolucion];

        for (int y = 0; y < resolucion; y++)
        {
            for (int x = 0; x < resolucion; x++)
            {
                int banda = (x + y) / anchoBanda;
                pixeles[y * resolucion + x] = (banda % 2 == 0) ? colorA : colorB;
            }
        }

        textura.SetPixels(pixeles);
        textura.Apply();
        return textura;
    }

    static Texture2D CrearTexturaAnillos(Color colorA, Color colorB, int anillos, int resolucion)
    {
        Texture2D textura = new Texture2D(resolucion, resolucion, TextureFormat.RGBA32, false);
        textura.filterMode = FilterMode.Point;
        textura.wrapMode = TextureWrapMode.Clamp;

        float centro = resolucion / 2f;
        float paso = centro / anillos;
        Color[] pixeles = new Color[resolucion * resolucion];

        for (int y = 0; y < resolucion; y++)
        {
            for (int x = 0; x < resolucion; x++)
            {
                float dx = x - centro;
                float dy = y - centro;
                float distancia = Mathf.Sqrt(dx * dx + dy * dy);
                int anillo = (int)(distancia / paso);
                pixeles[y * resolucion + x] = (anillo % 2 == 0) ? colorA : colorB;
            }
        }

        textura.SetPixels(pixeles);
        textura.Apply();
        return textura;
    }
}
