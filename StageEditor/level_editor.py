import bpy
import math
import bpy_extras
import gpu
import gpu_extras.batch
import copy
import mathutils
import json
import os

# ブレンダーに登録するアドオン情報
bl_info = {
	"name": "レベルエディタ",
	"author": "Takai Rikushi",
	"version": (1,0),
	"blender": (3,3,1),
	"location": "",
	"description": "レベルエディタ",
	"support": "TESTING",
	"wiki_uri": "",
	"category": "object"
}

#コライダー描画
class DrawCollider:
	#描画ハンドル
	handle = None

	#3Dビューに登録する描画関数
	def draw_collider():
		#頂点データ
		vertices = {"pos":[]}
		#インデックスデータ
		indices = []

		offsets = [
			[-0.5,-0.5,-0.5],
			[+0.5,-0.5,-0.5],
			[-0.5,+0.5,-0.5],
			[+0.5,+0.5,-0.5],
			[-0.5,-0.5,+0.5],
			[+0.5,-0.5,+0.5],
			[-0.5,+0.5,+0.5],
			[+0.5,+0.5,+0.5],
		]

		size = [2,2,2]

		#現在のシーンのオブジェクトの数だけ回す
		for object in bpy.context.scene.objects:
			#コライダープロパティがなければ、描画をスキップ
			if not "collider" in object:
				continue

			#変数宣言(中身の意味はなさそう)
			center = mathutils.Vector((0,0,0))
			size = mathutils.Vector((2,2,2))

			#プロパティから値を取得
			center[0]=object["collider_center"][0]
			center[1]=object["collider_center"][1]
			center[2]=object["collider_center"][2]
			size[0]=object["collider_size"][0]
			size[1]=object["collider_size"][1]
			size[2]=object["collider_size"][2]

			start = len(vertices["pos"])

			#Boxの8頂点分回す
			for offset in offsets:
				pos = copy.copy(center)

				pos[0]+=offset[0]*size[0]
				pos[1]+=offset[1]*size[1]
				pos[2]+=offset[2]*size[2]

				pos = object.matrix_world @ pos

				vertices['pos'].append(pos)

				indices.append([start + 0,start + 1])
				indices.append([start + 2,start + 3])
				indices.append([start + 0,start + 2])
				indices.append([start + 1,start + 3])

				indices.append([start + 4,start + 5])
				indices.append([start + 6,start + 7])
				indices.append([start + 4,start + 6])
				indices.append([start + 5,start + 7])

				indices.append([start + 0,start + 4])
				indices.append([start + 1,start + 5])
				indices.append([start + 2,start + 6])
				indices.append([start + 3,start + 7])
		
		#ビルドインのシェーダを取得
		shader = gpu.shader.from_builtin("3D_UNIFORM_COLOR")
		batch = gpu_extras.batch.batch_for_shader(shader,"LINES",vertices,indices = indices)
		#シェーダのパラメータ設定
		color = [0.5,1.0,1.0,1.0]
		shader.bind()
		shader.uniform_float("color",color)
		#描画
		batch.draw(shader)

#def draw_menu_manual(self,context):
#	self.layout.operator("wm.url_open_preset",text = "Manual",icon='HELP')

class OBJECT_PT_collider(bpy.types.Panel):
	bl_name = "OBJECT_PT_collider"
	bl_label = "collider"
	bl_space_type = "PROPERTIES"
	bl_region_type = "WINDOW"
	bl_context = "object"

	def draw(self,context):
		#サブメニューの描画
		if "collider" in context.object:
			#すでにプロパティがあれば、プロパティを表示
			self.layout.prop(context.object,'["collider"]',text="Type")
			self.layout.prop(context.object,'["collider_center"]',text="Center")
			self.layout.prop(context.object,'["collider_size"]',text="Size")
		else:
			#なければ、ボタンを表示
			self.layout.operator(MYADDON_OT_add_collider.bl_idname,
					text=MYADDON_OT_add_collider.bl_label)

#読み込み時にそこにオブジェクトを配置するタグを追加するクラス
class MYADDON_OT_add_setObject(bpy.types.Operator):
	bl_idname = "myaddon.myaddon_ot_add_setobject"
	bl_label = "直接配置オブジェクト 追加"
	bl_description = "['setObject']カスタムプロパティを追加します"
	bl_options = {"REGISTER","UNDO"}

	def execute(self,context):
		#['setObject']カスタムプロパティを追加
		context.object["setObject"] = ""

		return {"FINISHED"}

#使用するテクスチャを追加するクラス
class MYADDON_OT_add_Texture(bpy.types.Operator):
	bl_idname = "myaddon.myaddon_ot_add_texture"
	bl_label = "テクスチャ 追加"
	bl_description = "['texture']カスタムプロパティを追加します"
	bl_options = {"REGISTER","UNDO"}

	def execute(self,context):
		#['texture']カスタムプロパティを追加
		context.object["texture"] = ""

		return {"FINISHED"}

#コライダーを追加するクラス
class MYADDON_OT_add_collider(bpy.types.Operator):
	bl_idname = "myaddon.myaddon_ot_add_collider"
	bl_label = "コライダー 追加"
	bl_description = "['collider']カスタムプロパティを追加します"
	bl_options = {"REGISTER","UNDO"}

	def execute(self,context):
		#['collider']カスタムプロパティを追加
		context.object["collider"] = "BOX"
		context.object["collider_center"] = mathutils.Vector((0,0,0))
		context.object["collider_size"] = mathutils.Vector((2,2,2))

		return {"FINISHED"}
	
#イベントトリガーを追加するクラス
class MYADDON_OT_add_event_trigger(bpy.types.Operator):
	bl_idname = "myaddon.myaddon_ot_add_event_trigger"
	bl_label = "イベントトリガー 追加"
	bl_description = "['event_trigger']カスタムプロパティを追加します"
	bl_options = {"REGISTER","UNDO"}

	def execute(self,context):
		#['event_trigger']カスタムプロパティを追加
		context.object["event_trigger"] = ""

		return {"FINISHED"}

#ファイル名を追加するクラス
class MYADDON_OT_add_filename(bpy.types.Operator):
	bl_idname = "myaddon.myaddon_ot_add_filename"
	bl_label = "ファイル名 追加"
	bl_description = "['file_name'] カスタムプロパティを追加します"
	bl_options = {"REGISTER","UNDO"}

	def execute(self,context):
		context.object["file_name"] = ""
		
		return {"FINISHED"}

# パネル ファイル名
class OBJECT_PT_file_name(bpy.types.Panel):
	"""オブジェクトのファイルネームパネル"""
	bl_idname = "OBJECT_PT_file_name"
	bl_label = "FileName"
	bl_space_type = "PROPERTIES"
	bl_region_type = "WINDOW"
	bl_context = "object"

	#サブメニューの描画
	def draw(self,context):
		if "file_name" in context.object:
			#すでにプロパティがあれば、プロパティを表示
			self.layout.prop(context.object,'["file_name"]',text=self.bl_label)
		else:
			self.layout.operator(MYADDON_OT_add_filename.bl_idname,
					text=MYADDON_OT_add_filename.bl_label)
			
#テクスチャのタイリング数を決めるクラス
class MYADDON_OT_add_Tiling(bpy.types.Operator):
	bl_idname = "myaddon.myaddon_ot_add_tiling"
	bl_label = "タイリング数 追加"
	bl_description = "['tiling'] カスタムプロパティを追加します"
	bl_options = {"REGISTER","UNDO"}

	def execute(self,context):
		context.object["tiling"] = mathutils.Vector((1,1))
		
		return {"FINISHED"}

# パネル ファイル名
class OBJECT_PT_Tiling(bpy.types.Panel):
	"""オブジェクトのファイルネームパネル"""
	bl_idname = "OBJECT_PT_tiling"
	bl_label = "tiling"
	bl_space_type = "PROPERTIES"
	bl_region_type = "WINDOW"
	bl_context = "object"

	#サブメニューの描画
	def draw(self,context):
		if "tiling" in context.object:
			#すでにプロパティがあれば、プロパティを表示
			self.layout.prop(context.object,'["tiling"]',text="tiling")
		else:
			self.layout.operator(MYADDON_OT_add_Tiling.bl_idname,
					text=MYADDON_OT_add_Tiling.bl_label)

class OBJECT_PT_setObject(bpy.types.Panel):
	"""オブジェクトのファイルネームパネル"""
	bl_idname = "OBJECT_PT_setObject"
	bl_label = "setObject"
	bl_space_type = "PROPERTIES"
	bl_region_type = "WINDOW"
	bl_context = "object"

	#サブメニューの描画
	def draw(self,context):
		if "setObject" in context.object:
			#すでにプロパティがあれば、プロパティを表示
			self.layout.prop(context.object,'["setObject"]',text=self.bl_label)
		else:
			self.layout.operator(MYADDON_OT_add_setObject.bl_idname,
					text=MYADDON_OT_add_setObject.bl_label)

class OBJECT_PT_Texture(bpy.types.Panel):
	"""オブジェクトのファイルネームパネル"""
	bl_idname = "OBJECT_PT_texture"
	bl_label = "texture"
	bl_space_type = "PROPERTIES"
	bl_region_type = "WINDOW"
	bl_context = "object"

	#サブメニューの描画
	def draw(self,context):
		if "texture" in context.object:
			#すでにプロパティがあれば、プロパティを表示
			self.layout.prop(context.object,'["texture"]',text=self.bl_label)
		else:
			self.layout.operator(MYADDON_OT_add_Texture.bl_idname,
					text=MYADDON_OT_add_Texture.bl_label)
			
class OBJECT_PT_event_trigger(bpy.types.Panel):
	"""オブジェクトのファイルネームパネル"""
	bl_idname = "OBJECT_PT_event_trigger"
	bl_label = "event_trigger"
	bl_space_type = "PROPERTIES"
	bl_region_type = "WINDOW"
	bl_context = "object"

	#サブメニューの描画
	def draw(self,context):
		if "event_trigger" in context.object:
			#すでにプロパティがあれば、プロパティを表示
			self.layout.prop(context.object,'["event_trigger"]',text=self.bl_label)
		else:
			self.layout.operator(MYADDON_OT_add_event_trigger.bl_idname,
					text=MYADDON_OT_add_event_trigger.bl_label)

class MYADDON_OT_export_scene(bpy.types.Operator,bpy_extras.io_utils.ExportHelper):
	bl_idname = "myaddon.myaddon_ot_export_scene"
	bl_label = "シーン出力"
	bl_description = "シーン情報をExportします"
	#出力するファイルの拡張子
	filename_ext = ".json"

	def execute(self,context):

		print("シーン情報をExportします")

		self.export_json()

		print("シーン情報をExportしました")
		self.report({'INFO'},"シーン情報をExportしました")

		return {'FINISHED'}
				
	def export_json(self):
		"""JSON形式でファイルに出力"""

		#保存する情報をまとめるdict
		json_object_root = dict()

		#ノード名
		json_object_root["name"] = "scene"
		#オブジェクトリストを作成
		json_object_root["objects"] = list()
		
		#シーン内の全オブジェクトを走査してパック
		for object in bpy.context.scene.objects:
			#子供ならスキップ(代わりに親から呼び出すから)
			if(object.parent):
				continue

			#シーン直下のオブジェクトをルートノード(深さ0)とし、再起関数で走査
			self.parse_scene_recursive_json(json_object_root["objects"],object,0)

		#オブジェクトをJSON文字列にエンコード
		json_text = json.dumps(json_object_root,ensure_ascii=False,cls=json.JSONEncoder,indent=4)

		print(json_text)

		#ファイルをテキスト形式で書き出し用にオープン
		#スコープを抜けると自動的にクローズされる
		with open(self.filepath,"wt",encoding="utf-8")as file:

			#ファイルに文字列を書き込む
			file.write(json_text)

	def write_and_print(self,file,str):
		print(str)

		file.write(str)
		file.write('\n')

	def parse_scene_recursive_json(self,data_parent,object,level):
		#シーンのオブジェクト1個分のjsonオブジェクト生成
		json_object = dict()
		#オブジェクト種類
		json_object["type"] = object.type
		#オブジェクト名
		json_object["name"] = object.name

		#Todo:その他情報をパック
		#オブジェクトのローカルトランスフォームから
		#平行移動、回転、スケールを抽出
		#trans,rot,scale = object.matrix_local.decompose()
		trans,rot,scale = object.matrix_world.decompose()
		#回転をQuternionからEuler(3軸での回転角)に変換
		rot = rot.to_euler()
		#ラジアンから度数法に変換
		rot.x = math.degrees(rot.x)
		rot.y = math.degrees(rot.y)
		rot.z = math.degrees(rot.z)
		#トランスフォーム情報をディクショナリに登録
		transform = dict()
		transform["translation"] = (trans.x,trans.y,trans.z)
		transform["rotation"] = (rot.x,rot.y,rot.z)
		transform["scaling"] = (scale.x,scale.y,scale.z)
		#まとめて1個分のjsonオブジェクトに登録
		json_object["transform"] = transform

		#カスタムプロパティ'file_name'
		if "file_name" in object:
			json_object["file_name"] = object["file_name"]

		#カスタムプロパティ'collider'
		if "collider" in object:
			collider = dict()
			collider["type"] = object["collider"]
			collider["center"] = object["collider_center"].to_list()
			collider["size"] = object["collider_size"].to_list()
			json_object["collider"]=collider

		if "texture" in object:
			json_object["texture"] = object["texture"]

		if "event_trigger" in object:
			json_object["event_trigger"] = object["event_trigger"]

		if "setObject" in object:
			json_object["setObject"] = object["setObject"]

		if "tiling" in object:
			tiling = dict()
			tiling["tiling"] = object["tiling"].to_list()
			json_object["tiling"]=tiling

		#1個分のjsonオブジェクトを親オブジェクトに登録
		data_parent.append(json_object)

		#Todo:直接の子供リストを走査
		#子ノードがあれば
		if len(object.children) > 0:
			#子ノードリストを作成
			json_object["children"] = list()

			#子ノードへ進む(深さが1上がる)
			for child in object.children:
				self.parse_scene_recursive_json(json_object["children"],child,level + 1)

#大砲の配置
class MYADDON_OT_create_cannon(bpy.types.Operator):
	bl_idname = "myaddon.myaddon_ot_create_cannon"
	bl_label = "大砲オブジェクトを作成"
	bl_description = "大砲オブジェクトを作成します。中間点、終点の移動が必要です。"
	bl_options = {'REGISTER','UNDO'}

	#メニューを実行した時に呼ばれる関数
	def execute(self,context):
		filepath = bpy.data.filepath
		
		print("filepath = " + filepath)

		dirpath = os.path.dirname(filepath)
		print("dirpath = " + dirpath)
		bpy.ops.import_scene.obj(filepath=dirpath + "\\Resources\\Cannon\\Cannon.obj",axis_forward='-Z', axis_up='Y')
		import_obj = bpy.context.selected_objects[0]
		import_obj['file_name'] = 'Cannon'
		
		#軸変換による余計な回転が入っているので適用して解除
		bpy.ops.object.transform_apply(rotation=True)

		id = 0
		for check_obj in bpy.context.scene.objects:
        # オブジェクト名が指定の正規表現と一致するか
			if "大砲" in check_obj.name:
            # 一致すれば対象とする
				id += 1

		import_obj.name = "大砲"
		import_obj['id'] = id
		import_obj['event_trigger'] = "Cannon_start_" + str(id)
		import_obj["collider"] = "BOX"
		import_obj["collider_center"] = mathutils.Vector((0,0,0.5))
		import_obj["collider_size"] = mathutils.Vector((2,2,4))

		import_obj.location = context.scene.cursor.location
		import_obj.scale = mathutils.Vector((1,1,1))

		#中間点オブジェクトを追加
		bpy.ops.mesh.primitive_cube_add()
		inter_obj = bpy.context.selected_objects[0]
		inter_obj.name = "中間点"
		inter_obj['id'] = id
		inter_obj['event_trigger'] = "Cannon_inter_" + str(id)
		inter_obj["collider"] = "BOX"
		inter_obj["collider_center"] = mathutils.Vector((0,0,0))
		inter_obj["collider_size"] = mathutils.Vector((2,2,2))
		inter_obj.location = context.scene.cursor.location
		inter_obj.location.z += 3

		#終点オブジェクトを追加
		bpy.ops.mesh.primitive_cube_add()
		end_obj = bpy.context.selected_objects[0]
		end_obj.name = "終点"
		end_obj['id'] = id
		end_obj['event_trigger'] = "Cannon_end_" + str(id)
		end_obj["collider"] = "BOX"
		end_obj["collider_center"] = mathutils.Vector((0,0,0))
		end_obj["collider_size"] = mathutils.Vector((2,2,2))
		end_obj.location = context.scene.cursor.location
		end_obj.location.z += 6

		#オペレータの命令終了を通知
		return {'FINISHED'}


#土管の配置
class MYADDON_OT_create_Dokan(bpy.types.Operator):
	bl_idname = "myaddon.myaddon_ot_create_dokan"
	bl_label = "土管オブジェクトを配置"
	bl_description = "土管オブジェクトを配置します。移動先の設定が必要です。"
	bl_options = {'REGISTER','UNDO'}

	#メニューを実行した時に呼ばれる関数
	def execute(self,context):
		filepath = bpy.data.filepath
		
		print("filepath = " + filepath)

		dirpath = os.path.dirname(filepath)
		print("dirpath = " + dirpath)
		bpy.ops.import_scene.obj(filepath=dirpath + "\\Resources\\dokan\\dokan.obj",axis_forward='-Z', axis_up='Y')
		
		import_obj = bpy.context.selected_objects[0]
		import_obj['file_name'] = 'dokan'
		
		#軸変換による余計な回転が入っているので適用して解除
		bpy.ops.object.transform_apply(rotation=True)
		
		#名前に"土管"が含まれるオブジェクトを全て検索
		# 処理対象のオブジェクトリストを作成する
		id = 0
		for check_obj in bpy.context.scene.objects:
        # オブジェクト名が指定の正規表現と一致するか
			if "土管" in check_obj.name:
            # 一致すれば対象とする
				id += 1
		#土管オブジェクトのidを取得　全ての土管idを取得終わったら

		#ソートして一番大きい値を取得+1の値をidとして挿入

		import_obj.name = "土管"
		import_obj['id'] = id
		import_obj['setObject'] = "dokan_" + str(id)
		import_obj["collider"] = "BOX"
		import_obj["collider_center"] = mathutils.Vector((0,0,1))
		import_obj["collider_size"] = mathutils.Vector((2,2,2))

		import_obj.location = context.scene.cursor.location
		import_obj.scale = mathutils.Vector((2,2,2))

		#オペレータの命令終了を通知
		return {'FINISHED'}

#オペレータ 線の描画
class MYADDON_OT_create_ico_sphere(bpy.types.Operator):
	bl_idname = "myaddon.myaddon_ot_create_object"
	bl_label = "大砲オブジェクトに線を描画します"
	bl_description = "大砲オブジェクトに線を描画します"
	bl_options = {'REGISTER','UNDO'}

	#メニューを実行した時に呼ばれる関数
	def execute(self,context):
		#変数の設定
		path = bpy.data.curves.new('path','CURVE') #pathという名前のカーブを作成する
		curve = bpy.data.objects.new('Curve',path) #Curveという名前のオブジェクトを作成する
  
		context.scene.collection.objects.link(curve) #オブジェクト("Curve")を3Dビューポート(Scene)に追加する
		path.dimensions = '3D' #カーブの種類を決定する
		spline = path.splines.new('POLY') #BEZIERを追加する

		start = [0,0,0,1]
		inter = [0,0,0,1]
		end = [0,0,0,1]

		for object in context.scene.objects:
			if not "event_trigger" in object:
				continue
			if "Cannon" in object["event_trigger"]:
				if "start_1" in object["event_trigger"]:
					trans = object.location
					start = [trans.x,trans.y,trans.z,1]
				if "inter_1" in object["event_trigger"]:
					trans = object.location
					inter = [trans.x,trans.y,trans.z,1]
				if "end_1" in object["event_trigger"]:
					trans = object.location
					end = [trans.x,trans.y,trans.z,1]

		spPoints = [start,inter,end]

		spline.points.add(len(spPoints)-1)

		for i,o in enumerate(spline.points):
			o.co = spPoints[i]

		return {'FINISHED'}

#トップバーの拡張メニュー
class TOPBAR_MT_editor_menu(bpy.types.Menu):
	bl_idname = "TOPBAR_MT_editor_menu"

	bl_label = "エディタメニュー"

	bl_description = "拡張メニュー by " + bl_info["author"]

	def draw(self,context):
		self.layout.operator(MYADDON_OT_export_scene.bl_idname,
		       text=MYADDON_OT_export_scene.bl_label)
		self.layout.separator()
		self.layout.operator(MYADDON_OT_create_ico_sphere.bl_idname,
		       text=MYADDON_OT_create_ico_sphere.bl_label)
		self.layout.operator(MYADDON_OT_create_Dokan.bl_idname,
		       text=MYADDON_OT_create_Dokan.bl_label)
		self.layout.operator(MYADDON_OT_create_cannon.bl_idname,
		       text=MYADDON_OT_create_cannon.bl_label)
		


	def submenu(self,context):
		self.layout.menu(TOPBAR_MT_editor_menu.bl_idname)

classes = (
	MYADDON_OT_create_ico_sphere,
	MYADDON_OT_create_cannon,
	MYADDON_OT_export_scene,
	TOPBAR_MT_editor_menu,
	MYADDON_OT_add_filename,
	OBJECT_PT_file_name,
	MYADDON_OT_add_collider,
	OBJECT_PT_collider,
	MYADDON_OT_add_Texture,
	OBJECT_PT_Texture,
	MYADDON_OT_add_event_trigger,
	OBJECT_PT_event_trigger,
	MYADDON_OT_add_setObject,
	OBJECT_PT_setObject,
	MYADDON_OT_add_Tiling,
	OBJECT_PT_Tiling,
	MYADDON_OT_create_Dokan,
)

def register():
	#チェックボックス追加用のなんか
	bpy.types.Scene.my_checkbox_property = bpy.props.BoolProperty(name="my_checkbox_property")

	#Blenderにクラスを登録
	for cls in classes:
		bpy.utils.register_class(cls)

	#メニューに項目を追加
	bpy.types.TOPBAR_MT_editor_menus.append(TOPBAR_MT_editor_menu.submenu)
	#3Dビューに描画関数を追加
	DrawCollider.handle = bpy.types.SpaceView3D.draw_handler_add(DrawCollider.draw_collider,(),"WINDOW","POST_VIEW")
	
	print("レベルエディタが有効化されました。")

def unregister():
	#メニューから項目を削除
	bpy.types.TOPBAR_MT_editor_menus.remove(TOPBAR_MT_editor_menu.submenu)
	#3Dビューから描画関数を削除
	bpy.types.SpaceView3D.draw_handler_remove(DrawCollider.handle,"WINDOW")

	print("レベルエディタが無効化されました。")
	#Blenderからクラスを削除
	for cls in classes:
		bpy.utils.unregister_class(cls)