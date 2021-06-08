@echo off
C:\CEVA-ToolBox\V18_NeuPro\XM\cevatools\bin\ceva-elf-objcopy.exe -O binary -R .inttbl -R .code.internal.csl -R .code.external.csl -R .text -R .code.external.cdnn cdnn_xm6_app.elf data.bin

C:\CEVA-ToolBox\V18_NeuPro\XM\cevatools\bin\ceva-elf-objcopy.exe -O binary -j .inttbl -j .code.internal.csl -j .code.external.csl -j .text -j .code.external.cdnn cdnn_xm6_app.elf program.bin

wsl ./gen_prog_bin program.bin prog.bin
del program.bin
