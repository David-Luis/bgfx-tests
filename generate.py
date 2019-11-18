import os
import shutil

try:
	shutil.rmtree('./build/vs2017/shaders/')
except:
	print("")

try:
	os.mkdir("./build/vs2017/shaders/")
except:
	print("")
	
try:
	os.mkdir("./build/vs2017/textures/")
except:
	print("")
	
try:
	#enable when needed, check how shader.mk inside bgfx repo
	os.mkdir("./build/vs2017/shaders/dx9")
	os.mkdir("./build/vs2017/shaders/dx11")
	#os.mkdir("./build/vs2017/shaders/essl")
	#os.mkdir("./build/vs2017/shaders/glsl")
	#os.mkdir("./build/vs2017/shaders/metal")
	#os.mkdir("./build/vs2017/shaders/pssl")
	#os.mkdir("./build/vs2017/shaders/spirv")
except:
	print("")

os.system("premake5.exe vs2017")

def create_shader(name):
	#dx9
	#os.system(f'shadercDebug.exe -f "res/shaders/{name}_fs.sc" --varyingdef "res/shaders/{name}_varying.sc" -o "build/vs2017/shaders/dx9/{name}_fs.bin" --type f --platform windows -p ps_3_0 -O 3')
	#os.system(f'shadercDebug.exe -f "res/shaders/{name}_vs.sc" --varyingdef "res/shaders/{name}_varying.sc" -o "build/vs2017/shaders/dx9/{name}_vs.bin" --type v --platform windows -p vs_3_0 -O 3')

	#dx11
	os.system(f'shadercDebug.exe -f "res/shaders/{name}_fs.sc" --varyingdef "res/shaders/{name}_varying.sc" -o "build/vs2017/shaders/dx11/{name}_fs.bin" --type f --platform windows -p ps_5_0 -O 3')
	os.system(f'shadercDebug.exe -f "res/shaders/{name}_vs.sc" --varyingdef "res/shaders/{name}_varying.sc" -o "build/vs2017/shaders/dx11/{name}_vs.bin" --type v --platform windows -p vs_5_0 -O 3')
	
def create_texture(name):
	os.system(f'texturecDebug.exe -f "res/textures/{name}.tga" -o "build/vs2017/textures/{name}.dds"')
	
create_shader("color")
create_shader("time_color")
create_shader("noise")
create_shader("test")
create_shader("graphs")
create_shader("raymarching_and_sdf")
create_shader("textured")
create_shader("gray")
create_shader("texture_no_light")

create_texture("fieldstone-rgba")
create_texture("fieldstone-n")