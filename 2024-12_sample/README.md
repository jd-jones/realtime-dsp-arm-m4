To build:

```
./build.sh
```

To flash a device:
```
STM32_Programmer_CLI --connect port=SWD --write cmake-build-debug/2024-12_sample.elf --verify -rst
```
