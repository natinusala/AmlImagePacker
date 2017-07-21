# AmlImagePacker
Tool which packs a Amlogic ROM into a burnable IMG file.

Allows you to quickly pack a ROM (`tmp` folder) into an IMG without using Amlogic Customization Tool.

## Usage

Put `AmlImagePacker.exe`, the `tmp` folder and `AmlImagePack.dll` in the same folder. Run `AmlImagePacker.exe`. Your ROM will be packed into the `output.img` file.

## I want to change the output file name and/or the `tmp` folder path

Rebuild from sources and change the defines. The hacky way of loading the DLL prevents me from using char pointers, so I can't use command line arguments.
