#include "stdafx.h"
#include <Windows.h>

char* CAmlImagePack_constructor_MANGLED = "??0CAmlImagePack@@QAE@XZ";
char* AmlImagePack_pack_MANGLED = "?AmlImg_pack@CAmlImagePack@@QAEHPBD00H@Z";
char* AmlImagePack_check_MANGLED = "?AmlImg_check@CAmlImagePack@@QAEHPBD@Z";
char* CAmlImagePack_destructor_MANGLED = "??1CAmlImagePack@@QAE@XZ";

#define CFG "./tmp/level1/image.cfg"
#define LEVEL1 "./tmp/level1"
#define OUTPUT "output.img"

typedef void (WINAPI* CAmlImagePack_constructor)(); 
typedef int (WINAPI* AmlImagePack_pack)(const char* cfg, const char* level1, const char* output, int unknown);
typedef int (WINAPI* AmlImagePack_check)(const char* img);
typedef void (WINAPI* CAmlImagePack_destructor)();

//Inspired by https://www.codeproject.com/Articles/9405/Using-classes-exported-from-a-DLL-using-LoadLibrar
//Since it directly uses the ECX register it's not very portable but it should work on most x86 systems
//The hacky DLL loading prevents usage of parameters that are not litterals, so no argv for us

//Usage : copy both the AmlImagePack.dll file and the tmp folder containing the ROM in the same directory as this tool and run it
//        if everything went correctly, an output.img file should have been created - it's your image

int main(int argc, char** argv)
{
	//Library loading
	printf("[AMLImagePacker] Loading AmlImagePack.dll\n");
	HMODULE mod = LoadLibrary("AmlImagePack.dll");

	if (mod == NULL)
	{
		printf("[AMLImagePacker] Unable to load AmlImagePack.dll\n");
		return 1;
	}

	//Methods loading
	printf("[AMLImagePacker] Loading CAmlImagePack class methods\n");
	CAmlImagePack_constructor constructor = (CAmlImagePack_constructor)GetProcAddress(mod, CAmlImagePack_constructor_MANGLED);
	AmlImagePack_pack pack = (AmlImagePack_pack)GetProcAddress(mod, AmlImagePack_pack_MANGLED);
	AmlImagePack_check check = (AmlImagePack_check)GetProcAddress(mod, AmlImagePack_check_MANGLED);
	CAmlImagePack_destructor destructor = (CAmlImagePack_destructor)GetProcAddress(mod, CAmlImagePack_destructor_MANGLED);

	if (constructor == NULL || pack == NULL || check == NULL || destructor == NULL)
	{
		printf("[AMLImagePacker] Unable to load CAmlImagePack class methods from the DLL, is it the right DLL ?\n");
		printf("[AMLImagePacker] The DLL should weigh 57 344 bytes and have a CRC-32 of 4195D8AD\n");

		FreeLibrary(mod);

		return 2;
	}

	printf("[AMLImagePacker] Packing\n");

	//Allocate the class instance, Obj-C style
	void* thisPtr = (void*) malloc(32000000); //I don't know how large is the class so I just allocate 32mb of memory

	//Constructor
	_asm {mov ECX, thisPtr}
	constructor();

	//Image packing
	_asm {mov ECX, thisPtr}
	int result = pack(CFG, LEVEL1, OUTPUT, 2); //I don't know what the 2 means but I know that it should be 2
	
	if (result != 0)
	{
		printf("[AMLImagePacker] Failed to pack the image\n");

		//Destructor
		_asm {mov ECX, thisPtr}
		destructor();

		//Free the class instance
		free(thisPtr);

		//Free the library
		FreeLibrary(mod);

		return 3;
	}
	else
	{
		//Image check
		_asm {mov ECX, thisPtr}
		result = check(OUTPUT);

		printf("[AMLImagePacker] Image check result : %d\n", result);

		if (result != 0)
		{
			printf("[AMLImagePacker] Image check failed ! It may be corrupt.\n");
		}
	}
		
	//Destructor
	_asm {mov ECX, thisPtr}
	destructor();

	//Free the class instance
	free(thisPtr);

	//Free the library
	FreeLibrary(mod);

	printf("[AMLImagePacker] Done !\n");

    return 0;
}

