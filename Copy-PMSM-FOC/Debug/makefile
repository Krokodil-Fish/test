################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/ti/ccsv6/tools/compiler/ti-cgt-c2000_6.4.6

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./Src/128x64.obj" \
"./Src/DSP2833x_ADC_cal.obj" \
"./Src/DSP2833x_Adc.obj" \
"./Src/DSP2833x_CodeStartBranch.obj" \
"./Src/DSP2833x_CpuTimers.obj" \
"./Src/DSP2833x_DefaultIsr.obj" \
"./Src/DSP2833x_EPwm.obj" \
"./Src/DSP2833x_GlobalVariableDefs.obj" \
"./Src/DSP2833x_Gpio.obj" \
"./Src/DSP2833x_MemCopy.obj" \
"./Src/DSP2833x_PieCtrl.obj" \
"./Src/DSP2833x_PieVect.obj" \
"./Src/DSP2833x_Sci.obj" \
"./Src/DSP2833x_Spi.obj" \
"./Src/DSP2833x_SysCtrl.obj" \
"./Src/DSP2833x_usDelay.obj" \
"./Src/Dis.obj" \
"./Src/Main.obj" \
"./Src/TongXin.obj" \
"../Lib/IQmath_fpu32.lib" \
"../Lib/rts2800_fpu32.lib" \
"../Cmd/DSP2833x_Headers_nonBIOS.cmd" \
"../Cmd/F28335.cmd" \
$(GEN_CMDS__FLAG) \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include Src/subdir_vars.mk
-include Lib/subdir_vars.mk
-include Cmd/subdir_vars.mk
-include Src/subdir_rules.mk
-include Lib/subdir_rules.mk
-include Cmd/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
Copy\ of\ PMSM-FOC-Idsp.taobao.com.out \

EXE_OUTPUTS__QUOTED += \
"Copy of PMSM-FOC-Idsp.taobao.com.out" \


# All Target
all: Copy\ of\ PMSM-FOC-Idsp.taobao.com.out

# Tool invocations
Copy\ of\ PMSM-FOC-Idsp.taobao.com.out: $(OBJS) $(CMD_SRCS) $(LIB_SRCS) $(GEN_CMDS)
	@echo 'Building target: $@'
	@echo 'Invoking: C2000 Linker'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-c2000_6.4.6/bin/cl2000" -v28 -ml -mt --float_support=fpu32 -g --diag_warning=225 --display_error_number --diag_wrap=off -z -m"Copy of PMSM-FOC-Idsp.taobao.com.map" --stack_size=0x400 --warn_sections -i"C:/ti/ccsv6/tools/compiler/ti-cgt-c2000_6.4.6/lib" -i"C:/ti/ccsv6/tools/compiler/ti-cgt-c2000_6.4.6/include" --reread_libs --display_error_number --diag_wrap=off --xml_link_info="Copy of PMSM-FOC-Idsp.taobao.com_linkInfo.xml" --rom_model -o "Copy of PMSM-FOC-Idsp.taobao.com.out" $(ORDERED_OBJS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXE_OUTPUTS__QUOTED)
	-$(RM) "Src\128x64.pp" "Src\DSP2833x_Adc.pp" "Src\DSP2833x_CpuTimers.pp" "Src\DSP2833x_DefaultIsr.pp" "Src\DSP2833x_EPwm.pp" "Src\DSP2833x_GlobalVariableDefs.pp" "Src\DSP2833x_Gpio.pp" "Src\DSP2833x_MemCopy.pp" "Src\DSP2833x_PieCtrl.pp" "Src\DSP2833x_PieVect.pp" "Src\DSP2833x_Sci.pp" "Src\DSP2833x_Spi.pp" "Src\DSP2833x_SysCtrl.pp" "Src\Dis.pp" "Src\Main.pp" "Src\TongXin.pp" 
	-$(RM) "Src\128x64.obj" "Src\DSP2833x_ADC_cal.obj" "Src\DSP2833x_Adc.obj" "Src\DSP2833x_CodeStartBranch.obj" "Src\DSP2833x_CpuTimers.obj" "Src\DSP2833x_DefaultIsr.obj" "Src\DSP2833x_EPwm.obj" "Src\DSP2833x_GlobalVariableDefs.obj" "Src\DSP2833x_Gpio.obj" "Src\DSP2833x_MemCopy.obj" "Src\DSP2833x_PieCtrl.obj" "Src\DSP2833x_PieVect.obj" "Src\DSP2833x_Sci.obj" "Src\DSP2833x_Spi.obj" "Src\DSP2833x_SysCtrl.obj" "Src\DSP2833x_usDelay.obj" "Src\Dis.obj" "Src\Main.obj" "Src\TongXin.obj" 
	-$(RM) "Src\DSP2833x_ADC_cal.pp" "Src\DSP2833x_CodeStartBranch.pp" "Src\DSP2833x_usDelay.pp" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

