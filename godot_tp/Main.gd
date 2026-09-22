extends Node3D
var planos_pivot: Node3D
var gema: Node3D
var mat_plano_a: StandardMaterial3D
var mat_plano_b: StandardMaterial3D

const VELOCIDAD_BASE := 0.9

var eje_rotacion := Vector3.UP
var velocidad_actual := VELOCIDAD_BASE 
var contador_patron := 0

var paletas := [
	[Color(1.0, 0.45, 0.0), Color(0.15, 0.85, 0.2)],  # naranja / verde
	[Color(0.85, 0.1, 0.15), Color(0.95, 0.85, 0.2)], # rojo / amarillo
	[Color(0.1, 0.55, 0.95), Color(0.9, 0.9, 0.95)],  # azul / blanco
]


func _ready() -> void:
	_armar_entorno()
	_armar_camara()
	_armar_luces()
	_armar_gema()
	_armar_planos()
	_armar_interfaz()
	_aplicar_patron()


func _armar_entorno() -> void:
	var env := Environment.new()
	env.background_mode = Environment.BG_COLOR
	env.background_color = Color(0, 0, 0)
	env.ambient_light_source = Environment.AMBIENT_SOURCE_COLOR
	env.ambient_light_color = Color(0.08, 0.08, 0.1)
	env.ambient_light_energy = 1.0

	var world_env := WorldEnvironment.new()
	world_env.environment = env
	add_child(world_env)


func _armar_camara() -> void:
	var camara := Camera3D.new()
	add_child(camara)
	camara.position = Vector3(0, 3.2, 9.5)
	camara.current = true
	camara.look_at(Vector3.ZERO, Vector3.UP)


func _armar_luces() -> void:
	var clave := DirectionalLight3D.new()
	clave.rotation_degrees = Vector3(-45, -35, 0)
	clave.light_energy = 1.1
	add_child(clave)

	var relleno := OmniLight3D.new()
	relleno.position = Vector3(-4, 1.5, 4)
	relleno.light_energy = 3.0
	relleno.light_color = Color(0.35, 0.65, 1.0)
	relleno.omni_range = 14.0
	add_child(relleno)


func _armar_gema() -> void:
	gema = Node3D.new()
	gema.name = "Gema"
	add_child(gema)

	var material := StandardMaterial3D.new()
	material.albedo_color = Color(0.15, 0.85, 0.9)
	material.metallic = 0.65
	material.roughness = 0.12
	material.rim_enabled = true
	material.rim = 0.5

	var cono_superior := CylinderMesh.new()
	cono_superior.top_radius = 0.0
	cono_superior.bottom_radius = 1.1
	cono_superior.height = 1.4
	cono_superior.radial_segments = 8
	var inst_superior := MeshInstance3D.new()
	inst_superior.mesh = cono_superior
	inst_superior.position = Vector3(0, 0.7, 0)
	inst_superior.material_override = material
	gema.add_child(inst_superior)

	var cono_inferior := CylinderMesh.new()
	cono_inferior.top_radius = 1.1
	cono_inferior.bottom_radius = 0.0
	cono_inferior.height = 1.4
	cono_inferior.radial_segments = 8
	var inst_inferior := MeshInstance3D.new()
	inst_inferior.mesh = cono_inferior
	inst_inferior.position = Vector3(0, -0.7, 0)
	inst_inferior.material_override = material
	gema.add_child(inst_inferior)


func _armar_planos() -> void:
	planos_pivot = Node3D.new()
	planos_pivot.name = "PlanosPivot"
	add_child(planos_pivot)

	var malla := PlaneMesh.new()
	malla.size = Vector2(7.0, 2.2)
	malla.subdivide_width = 6
	malla.subdivide_depth = 2

	mat_plano_a = StandardMaterial3D.new()
	mat_plano_a.cull_mode = BaseMaterial3D.CULL_DISABLED
	mat_plano_a.roughness = 0.8

	mat_plano_b = StandardMaterial3D.new()
	mat_plano_b.cull_mode = BaseMaterial3D.CULL_DISABLED
	mat_plano_b.roughness = 0.8

	var plano_a := MeshInstance3D.new()
	plano_a.mesh = malla
	plano_a.rotation_degrees = Vector3(90, 0, 0)
	plano_a.material_override = mat_plano_a
	planos_pivot.add_child(plano_a)

	var plano_b := MeshInstance3D.new()
	plano_b.mesh = malla
	plano_b.rotation_degrees = Vector3(90, 65, 0)
	plano_b.material_override = mat_plano_b
	planos_pivot.add_child(plano_b)


func _armar_interfaz() -> void:
	var capa := CanvasLayer.new()
	add_child(capa)
	var etiqueta := Label.new()
	etiqueta.text = "Flechas: eje/sentido de rotación   |   D: detener   |   R: reiniciar (cambia el patrón)"
	etiqueta.position = Vector2(16, 16)
	etiqueta.add_theme_color_override("font_color", Color(1, 1, 1))
	capa.add_child(etiqueta)


func _unhandled_input(event: InputEvent) -> void:
	if event is InputEventKey and event.pressed and not event.echo:
		match event.keycode:
			KEY_UP:
				eje_rotacion = Vector3.RIGHT
				velocidad_actual = VELOCIDAD_BASE
			KEY_DOWN:
				eje_rotacion = Vector3.RIGHT
				velocidad_actual = -VELOCIDAD_BASE
			KEY_LEFT:
				eje_rotacion = Vector3.UP
				velocidad_actual = VELOCIDAD_BASE
			KEY_RIGHT:
				eje_rotacion = Vector3.UP
				velocidad_actual = -VELOCIDAD_BASE
			KEY_D:
				velocidad_actual = 0.0
			KEY_R:
				_reiniciar()



func _process(delta: float) -> void:
	if velocidad_actual != 0.0:
		planos_pivot.rotate(eje_rotacion.normalized(), velocidad_actual * delta)
	gema.rotate(Vector3.UP, 0.15 * delta)


func _reiniciar() -> void:
	planos_pivot.rotation = Vector3.ZERO
	eje_rotacion = Vector3.UP
	velocidad_actual = VELOCIDAD_BASE
	contador_patron += 1
	_aplicar_patron()



func _aplicar_patron() -> void:
	var tipo := contador_patron % 3
	var paleta: Array = paletas[(contador_patron / 3) % paletas.size()]
	var tex_a: ImageTexture
	var tex_b: ImageTexture
	match tipo:
		0:
			tex_a = _textura_ajedrez(paleta[0], paleta[1])
			tex_b = _textura_ajedrez(paleta[1], paleta[0])
		1:
			tex_a = _textura_franjas(paleta[0], paleta[1])
			tex_b = _textura_franjas(paleta[1], paleta[0])
		_:
			tex_a = _textura_anillos(paleta[0], paleta[1])
			tex_b = _textura_anillos(paleta[1], paleta[0])
	mat_plano_a.albedo_texture = tex_a
	mat_plano_b.albedo_texture = tex_b


func _textura_ajedrez(color_a: Color, color_b: Color, celdas: int = 8, res: int = 128) -> ImageTexture:
	var img := Image.create_empty(res, res, false, Image.FORMAT_RGBA8)
	var tam_celda := res / celdas
	for y in res:
		for x in res:
			var cx := x / tam_celda
			var cy := y / tam_celda
			img.set_pixel(x, y, color_a if (cx + cy) % 2 == 0 else color_b)
	return ImageTexture.create_from_image(img)


func _textura_franjas(color_a: Color, color_b: Color, franjas: int = 10, res: int = 128) -> ImageTexture:
	var img := Image.create_empty(res, res, false, Image.FORMAT_RGBA8)
	var ancho_banda := res / franjas
	for y in res:
		for x in res:
			var banda := (x + y) / ancho_banda
			img.set_pixel(x, y, color_a if banda % 2 == 0 else color_b)
	return ImageTexture.create_from_image(img)


func _textura_anillos(color_a: Color, color_b: Color, anillos: int = 6, res: int = 128) -> ImageTexture:
	var img := Image.create_empty(res, res, false, Image.FORMAT_RGBA8)
	var centro := res / 2.0
	var paso := centro / anillos
	for y in res:
		for x in res:
			var d := Vector2(x - centro, y - centro).length()
			var anillo := int(d / paso)
			img.set_pixel(x, y, color_a if anillo % 2 == 0 else color_b)
	return ImageTexture.create_from_image(img)
