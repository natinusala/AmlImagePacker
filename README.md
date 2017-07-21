# AmlImagePacker
Tool which packs a Amlogic ROM into a burnable IMG file.

Allows you to quickly pack a level 1 ROM (`./tmp/level1` folder) into an IMG without using the Amlogic Customization Tool.

**Warning** : it only takes the `level1` folder and packs it into a burnable IMG file ! It **doesn't** (yet) update the `level1` partition files with the content in the `level2` folder. 

You can do it for the `system` partition by running `Aml_make_ext4fs.exe -l 2048m -s -a system ./tmp/level1/system.PARTITION ./tmp/level2/system/` before running this tool. This will take the `./tmp/level2/system` folder, pack it into the `./tmp/level1/system.PARTITION` file and create a burnable IMG. Updating the others partitions is a little bit more complicated.

## Usage

Put `AmlImagePacker.exe`, the `tmp` folder and `AmlImagePack.dll` in the same folder. Run `AmlImagePacker.exe`. Your ROM will be packed into the `output.img` file.

If it doesn't work, it's probably because the tool can't write to the output file. Running it as administrator should fix most of the issues.

## I want to change the output file name and/or the `tmp` folder path

Rebuild from sources and change the defines. The hacky way of loading the DLL prevents me from using char pointers, so I can't use command line arguments.
