#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/Sensor_Controller.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/Sensor_Controller.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=mcc_generated_files/spi/src/spi0.c mcc_generated_files/system/src/pins.c mcc_generated_files/system/src/protected_io.S mcc_generated_files/system/src/interrupt.c mcc_generated_files/system/src/system.c mcc_generated_files/system/src/config_bits.c mcc_generated_files/system/src/clock.c mcc_generated_files/timer/src/tcb0.c mcc_generated_files/timer/src/rtc.c mcc_generated_files/uart/src/usart2.c mcc_generated_files/uart/src/usart1.c main.c functions/modbus.c functions/debug_uart2.c functions/temp_sensors.c functions/system_registers.c DS.c dht11.c mcc_generated_files/timer/src/tcb2.c mcc_generated_files/timer/src/tca0.c mcc_generated_files/timer/src/tcb1.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/mcc_generated_files/spi/src/spi0.o ${OBJECTDIR}/mcc_generated_files/system/src/pins.o ${OBJECTDIR}/mcc_generated_files/system/src/protected_io.o ${OBJECTDIR}/mcc_generated_files/system/src/interrupt.o ${OBJECTDIR}/mcc_generated_files/system/src/system.o ${OBJECTDIR}/mcc_generated_files/system/src/config_bits.o ${OBJECTDIR}/mcc_generated_files/system/src/clock.o ${OBJECTDIR}/mcc_generated_files/timer/src/tcb0.o ${OBJECTDIR}/mcc_generated_files/timer/src/rtc.o ${OBJECTDIR}/mcc_generated_files/uart/src/usart2.o ${OBJECTDIR}/mcc_generated_files/uart/src/usart1.o ${OBJECTDIR}/main.o ${OBJECTDIR}/functions/modbus.o ${OBJECTDIR}/functions/debug_uart2.o ${OBJECTDIR}/functions/temp_sensors.o ${OBJECTDIR}/functions/system_registers.o ${OBJECTDIR}/DS.o ${OBJECTDIR}/dht11.o ${OBJECTDIR}/mcc_generated_files/timer/src/tcb2.o ${OBJECTDIR}/mcc_generated_files/timer/src/tca0.o ${OBJECTDIR}/mcc_generated_files/timer/src/tcb1.o
POSSIBLE_DEPFILES=${OBJECTDIR}/mcc_generated_files/spi/src/spi0.o.d ${OBJECTDIR}/mcc_generated_files/system/src/pins.o.d ${OBJECTDIR}/mcc_generated_files/system/src/protected_io.o.d ${OBJECTDIR}/mcc_generated_files/system/src/interrupt.o.d ${OBJECTDIR}/mcc_generated_files/system/src/system.o.d ${OBJECTDIR}/mcc_generated_files/system/src/config_bits.o.d ${OBJECTDIR}/mcc_generated_files/system/src/clock.o.d ${OBJECTDIR}/mcc_generated_files/timer/src/tcb0.o.d ${OBJECTDIR}/mcc_generated_files/timer/src/rtc.o.d ${OBJECTDIR}/mcc_generated_files/uart/src/usart2.o.d ${OBJECTDIR}/mcc_generated_files/uart/src/usart1.o.d ${OBJECTDIR}/main.o.d ${OBJECTDIR}/functions/modbus.o.d ${OBJECTDIR}/functions/debug_uart2.o.d ${OBJECTDIR}/functions/temp_sensors.o.d ${OBJECTDIR}/functions/system_registers.o.d ${OBJECTDIR}/DS.o.d ${OBJECTDIR}/dht11.o.d ${OBJECTDIR}/mcc_generated_files/timer/src/tcb2.o.d ${OBJECTDIR}/mcc_generated_files/timer/src/tca0.o.d ${OBJECTDIR}/mcc_generated_files/timer/src/tcb1.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/mcc_generated_files/spi/src/spi0.o ${OBJECTDIR}/mcc_generated_files/system/src/pins.o ${OBJECTDIR}/mcc_generated_files/system/src/protected_io.o ${OBJECTDIR}/mcc_generated_files/system/src/interrupt.o ${OBJECTDIR}/mcc_generated_files/system/src/system.o ${OBJECTDIR}/mcc_generated_files/system/src/config_bits.o ${OBJECTDIR}/mcc_generated_files/system/src/clock.o ${OBJECTDIR}/mcc_generated_files/timer/src/tcb0.o ${OBJECTDIR}/mcc_generated_files/timer/src/rtc.o ${OBJECTDIR}/mcc_generated_files/uart/src/usart2.o ${OBJECTDIR}/mcc_generated_files/uart/src/usart1.o ${OBJECTDIR}/main.o ${OBJECTDIR}/functions/modbus.o ${OBJECTDIR}/functions/debug_uart2.o ${OBJECTDIR}/functions/temp_sensors.o ${OBJECTDIR}/functions/system_registers.o ${OBJECTDIR}/DS.o ${OBJECTDIR}/dht11.o ${OBJECTDIR}/mcc_generated_files/timer/src/tcb2.o ${OBJECTDIR}/mcc_generated_files/timer/src/tca0.o ${OBJECTDIR}/mcc_generated_files/timer/src/tcb1.o

# Source Files
SOURCEFILES=mcc_generated_files/spi/src/spi0.c mcc_generated_files/system/src/pins.c mcc_generated_files/system/src/protected_io.S mcc_generated_files/system/src/interrupt.c mcc_generated_files/system/src/system.c mcc_generated_files/system/src/config_bits.c mcc_generated_files/system/src/clock.c mcc_generated_files/timer/src/tcb0.c mcc_generated_files/timer/src/rtc.c mcc_generated_files/uart/src/usart2.c mcc_generated_files/uart/src/usart1.c main.c functions/modbus.c functions/debug_uart2.c functions/temp_sensors.c functions/system_registers.c DS.c dht11.c mcc_generated_files/timer/src/tcb2.c mcc_generated_files/timer/src/tca0.c mcc_generated_files/timer/src/tcb1.c



CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk ${DISTDIR}/Sensor_Controller.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=AVR128DB32
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/mcc_generated_files/spi/src/spi0.o: mcc_generated_files/spi/src/spi0.c  .generated_files/flags/default/e8a185f0b9a175ca23fb8ae08885ba962bd6ebfc .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files/spi/src" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/spi/src/spi0.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/spi/src/spi0.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O2 -Og -ffunction-sections -fdata-sections -fshort-enums -fcommon -funsigned-char -funsigned-bitfields -DF_CPU=16000000UL -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mconst-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/mcc_generated_files/spi/src/spi0.o.d" -MT "${OBJECTDIR}/mcc_generated_files/spi/src/spi0.o.d" -MT ${OBJECTDIR}/mcc_generated_files/spi/src/spi0.o -o ${OBJECTDIR}/mcc_generated_files/spi/src/spi0.o mcc_generated_files/spi/src/spi0.c 
	
${OBJECTDIR}/mcc_generated_files/system/src/pins.o: mcc_generated_files/system/src/pins.c  .generated_files/flags/default/becfdce95fdfeae5aa0c7e4dcfab8be203c8673d .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files/system/src" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/system/src/pins.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/system/src/pins.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O2 -Og -ffunction-sections -fdata-sections -fshort-enums -fcommon -funsigned-char -funsigned-bitfields -DF_CPU=16000000UL -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mconst-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/mcc_generated_files/system/src/pins.o.d" -MT "${OBJECTDIR}/mcc_generated_files/system/src/pins.o.d" -MT ${OBJECTDIR}/mcc_generated_files/system/src/pins.o -o ${OBJECTDIR}/mcc_generated_files/system/src/pins.o mcc_generated_files/system/src/pins.c 
	
${OBJECTDIR}/mcc_generated_files/system/src/interrupt.o: mcc_generated_files/system/src/interrupt.c  .generated_files/flags/default/d08dce8414d92a8160b4c411dc59711cfd861ef7 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files/system/src" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/system/src/interrupt.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/system/src/interrupt.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O2 -Og -ffunction-sections -fdata-sections -fshort-enums -fcommon -funsigned-char -funsigned-bitfields -DF_CPU=16000000UL -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mconst-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/mcc_generated_files/system/src/interrupt.o.d" -MT "${OBJECTDIR}/mcc_generated_files/system/src/interrupt.o.d" -MT ${OBJECTDIR}/mcc_generated_files/system/src/interrupt.o -o ${OBJECTDIR}/mcc_generated_files/system/src/interrupt.o mcc_generated_files/system/src/interrupt.c 
	
${OBJECTDIR}/mcc_generated_files/system/src/system.o: mcc_generated_files/system/src/system.c  .generated_files/flags/default/86ed2e1deb75bc83cbefec0c3484d080a900308b .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files/system/src" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/system/src/system.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/system/src/system.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O2 -Og -ffunction-sections -fdata-sections -fshort-enums -fcommon -funsigned-char -funsigned-bitfields -DF_CPU=16000000UL -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mconst-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/mcc_generated_files/system/src/system.o.d" -MT "${OBJECTDIR}/mcc_generated_files/system/src/system.o.d" -MT ${OBJECTDIR}/mcc_generated_files/system/src/system.o -o ${OBJECTDIR}/mcc_generated_files/system/src/system.o mcc_generated_files/system/src/system.c 
	
${OBJECTDIR}/mcc_generated_files/system/src/config_bits.o: mcc_generated_files/system/src/config_bits.c  .generated_files/flags/default/5d47369a389d90da75e9e6ed167ad449995730b2 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files/system/src" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/system/src/config_bits.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/system/src/config_bits.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O2 -Og -ffunction-sections -fdata-sections -fshort-enums -fcommon -funsigned-char -funsigned-bitfields -DF_CPU=16000000UL -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mconst-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/mcc_generated_files/system/src/config_bits.o.d" -MT "${OBJECTDIR}/mcc_generated_files/system/src/config_bits.o.d" -MT ${OBJECTDIR}/mcc_generated_files/system/src/config_bits.o -o ${OBJECTDIR}/mcc_generated_files/system/src/config_bits.o mcc_generated_files/system/src/config_bits.c 
	
${OBJECTDIR}/mcc_generated_files/system/src/clock.o: mcc_generated_files/system/src/clock.c  .generated_files/flags/default/753efaebe3771b416d3cf4fca4ae59ed38b1eb08 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files/system/src" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/system/src/clock.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/system/src/clock.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O2 -Og -ffunction-sections -fdata-sections -fshort-enums -fcommon -funsigned-char -funsigned-bitfields -DF_CPU=16000000UL -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mconst-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/mcc_generated_files/system/src/clock.o.d" -MT "${OBJECTDIR}/mcc_generated_files/system/src/clock.o.d" -MT ${OBJECTDIR}/mcc_generated_files/system/src/clock.o -o ${OBJECTDIR}/mcc_generated_files/system/src/clock.o mcc_generated_files/system/src/clock.c 
	
${OBJECTDIR}/mcc_generated_files/timer/src/tcb0.o: mcc_generated_files/timer/src/tcb0.c  .generated_files/flags/default/cc2e2e48e7aff427594c467d5cc8f8cdb7c6804f .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files/timer/src" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/timer/src/tcb0.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/timer/src/tcb0.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O2 -Og -ffunction-sections -fdata-sections -fshort-enums -fcommon -funsigned-char -funsigned-bitfields -DF_CPU=16000000UL -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mconst-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/mcc_generated_files/timer/src/tcb0.o.d" -MT "${OBJECTDIR}/mcc_generated_files/timer/src/tcb0.o.d" -MT ${OBJECTDIR}/mcc_generated_files/timer/src/tcb0.o -o ${OBJECTDIR}/mcc_generated_files/timer/src/tcb0.o mcc_generated_files/timer/src/tcb0.c 
	
${OBJECTDIR}/mcc_generated_files/timer/src/rtc.o: mcc_generated_files/timer/src/rtc.c  .generated_files/flags/default/8dec03a981821856d48200c134fbf1b0e30de795 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files/timer/src" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/timer/src/rtc.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/timer/src/rtc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O2 -Og -ffunction-sections -fdata-sections -fshort-enums -fcommon -funsigned-char -funsigned-bitfields -DF_CPU=16000000UL -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mconst-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/mcc_generated_files/timer/src/rtc.o.d" -MT "${OBJECTDIR}/mcc_generated_files/timer/src/rtc.o.d" -MT ${OBJECTDIR}/mcc_generated_files/timer/src/rtc.o -o ${OBJECTDIR}/mcc_generated_files/timer/src/rtc.o mcc_generated_files/timer/src/rtc.c 
	
${OBJECTDIR}/mcc_generated_files/uart/src/usart2.o: mcc_generated_files/uart/src/usart2.c  .generated_files/flags/default/340c41a5fb52048752add07b09c5dafac9456cbd .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files/uart/src" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/uart/src/usart2.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/uart/src/usart2.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O2 -Og -ffunction-sections -fdata-sections -fshort-enums -fcommon -funsigned-char -funsigned-bitfields -DF_CPU=16000000UL -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mconst-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/mcc_generated_files/uart/src/usart2.o.d" -MT "${OBJECTDIR}/mcc_generated_files/uart/src/usart2.o.d" -MT ${OBJECTDIR}/mcc_generated_files/uart/src/usart2.o -o ${OBJECTDIR}/mcc_generated_files/uart/src/usart2.o mcc_generated_files/uart/src/usart2.c 
	
${OBJECTDIR}/mcc_generated_files/uart/src/usart1.o: mcc_generated_files/uart/src/usart1.c  .generated_files/flags/default/78d72133866bfd5e348ba37dedacc1a472c22b7c .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files/uart/src" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/uart/src/usart1.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/uart/src/usart1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O2 -Og -ffunction-sections -fdata-sections -fshort-enums -fcommon -funsigned-char -funsigned-bitfields -DF_CPU=16000000UL -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mconst-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/mcc_generated_files/uart/src/usart1.o.d" -MT "${OBJECTDIR}/mcc_generated_files/uart/src/usart1.o.d" -MT ${OBJECTDIR}/mcc_generated_files/uart/src/usart1.o -o ${OBJECTDIR}/mcc_generated_files/uart/src/usart1.o mcc_generated_files/uart/src/usart1.c 
	
${OBJECTDIR}/main.o: main.c  .generated_files/flags/default/dcd3e4e82e1bdcefca2ee08041d5d5c65d4c39f9 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O2 -Og -ffunction-sections -fdata-sections -fshort-enums -fcommon -funsigned-char -funsigned-bitfields -DF_CPU=16000000UL -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mconst-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/main.o.d" -MT "${OBJECTDIR}/main.o.d" -MT ${OBJECTDIR}/main.o -o ${OBJECTDIR}/main.o main.c 
	
${OBJECTDIR}/functions/modbus.o: functions/modbus.c  .generated_files/flags/default/69499087779b629d411325c1165d7ee558d61b58 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/functions" 
	@${RM} ${OBJECTDIR}/functions/modbus.o.d 
	@${RM} ${OBJECTDIR}/functions/modbus.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O2 -Og -ffunction-sections -fdata-sections -fshort-enums -fcommon -funsigned-char -funsigned-bitfields -DF_CPU=16000000UL -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mconst-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/functions/modbus.o.d" -MT "${OBJECTDIR}/functions/modbus.o.d" -MT ${OBJECTDIR}/functions/modbus.o -o ${OBJECTDIR}/functions/modbus.o functions/modbus.c 
	
${OBJECTDIR}/functions/debug_uart2.o: functions/debug_uart2.c  .generated_files/flags/default/951b02f41164955245059529d889bb55ce350c88 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/functions" 
	@${RM} ${OBJECTDIR}/functions/debug_uart2.o.d 
	@${RM} ${OBJECTDIR}/functions/debug_uart2.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O2 -Og -ffunction-sections -fdata-sections -fshort-enums -fcommon -funsigned-char -funsigned-bitfields -DF_CPU=16000000UL -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mconst-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/functions/debug_uart2.o.d" -MT "${OBJECTDIR}/functions/debug_uart2.o.d" -MT ${OBJECTDIR}/functions/debug_uart2.o -o ${OBJECTDIR}/functions/debug_uart2.o functions/debug_uart2.c 
	
${OBJECTDIR}/functions/temp_sensors.o: functions/temp_sensors.c  .generated_files/flags/default/f76dab0eabfa34d9de0b79c665882f437d894d3d .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/functions" 
	@${RM} ${OBJECTDIR}/functions/temp_sensors.o.d 
	@${RM} ${OBJECTDIR}/functions/temp_sensors.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O2 -Og -ffunction-sections -fdata-sections -fshort-enums -fcommon -funsigned-char -funsigned-bitfields -DF_CPU=16000000UL -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mconst-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/functions/temp_sensors.o.d" -MT "${OBJECTDIR}/functions/temp_sensors.o.d" -MT ${OBJECTDIR}/functions/temp_sensors.o -o ${OBJECTDIR}/functions/temp_sensors.o functions/temp_sensors.c 
	
${OBJECTDIR}/functions/system_registers.o: functions/system_registers.c  .generated_files/flags/default/7dcad65a43e8b4e744b60e409660cb4654ebb5a6 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/functions" 
	@${RM} ${OBJECTDIR}/functions/system_registers.o.d 
	@${RM} ${OBJECTDIR}/functions/system_registers.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O2 -Og -ffunction-sections -fdata-sections -fshort-enums -fcommon -funsigned-char -funsigned-bitfields -DF_CPU=16000000UL -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mconst-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/functions/system_registers.o.d" -MT "${OBJECTDIR}/functions/system_registers.o.d" -MT ${OBJECTDIR}/functions/system_registers.o -o ${OBJECTDIR}/functions/system_registers.o functions/system_registers.c 
	
${OBJECTDIR}/DS.o: DS.c  .generated_files/flags/default/54b70115bc797bd3ff940568b000c084e06e7150 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/DS.o.d 
	@${RM} ${OBJECTDIR}/DS.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O2 -Og -ffunction-sections -fdata-sections -fshort-enums -fcommon -funsigned-char -funsigned-bitfields -DF_CPU=16000000UL -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mconst-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/DS.o.d" -MT "${OBJECTDIR}/DS.o.d" -MT ${OBJECTDIR}/DS.o -o ${OBJECTDIR}/DS.o DS.c 
	
${OBJECTDIR}/dht11.o: dht11.c  .generated_files/flags/default/f4ef8d573c29ac8f29069d77ae1dc9b0a1132a83 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/dht11.o.d 
	@${RM} ${OBJECTDIR}/dht11.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O2 -Og -ffunction-sections -fdata-sections -fshort-enums -fcommon -funsigned-char -funsigned-bitfields -DF_CPU=16000000UL -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mconst-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/dht11.o.d" -MT "${OBJECTDIR}/dht11.o.d" -MT ${OBJECTDIR}/dht11.o -o ${OBJECTDIR}/dht11.o dht11.c 
	
${OBJECTDIR}/mcc_generated_files/timer/src/tcb2.o: mcc_generated_files/timer/src/tcb2.c  .generated_files/flags/default/70c4d6ce6872a095687408d8470e6735f5d0e66e .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files/timer/src" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/timer/src/tcb2.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/timer/src/tcb2.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O2 -Og -ffunction-sections -fdata-sections -fshort-enums -fcommon -funsigned-char -funsigned-bitfields -DF_CPU=16000000UL -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mconst-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/mcc_generated_files/timer/src/tcb2.o.d" -MT "${OBJECTDIR}/mcc_generated_files/timer/src/tcb2.o.d" -MT ${OBJECTDIR}/mcc_generated_files/timer/src/tcb2.o -o ${OBJECTDIR}/mcc_generated_files/timer/src/tcb2.o mcc_generated_files/timer/src/tcb2.c 
	
${OBJECTDIR}/mcc_generated_files/timer/src/tca0.o: mcc_generated_files/timer/src/tca0.c  .generated_files/flags/default/5de9ff146dade69e4c48a82a8da1348a687a71a8 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files/timer/src" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/timer/src/tca0.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/timer/src/tca0.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O2 -Og -ffunction-sections -fdata-sections -fshort-enums -fcommon -funsigned-char -funsigned-bitfields -DF_CPU=16000000UL -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mconst-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/mcc_generated_files/timer/src/tca0.o.d" -MT "${OBJECTDIR}/mcc_generated_files/timer/src/tca0.o.d" -MT ${OBJECTDIR}/mcc_generated_files/timer/src/tca0.o -o ${OBJECTDIR}/mcc_generated_files/timer/src/tca0.o mcc_generated_files/timer/src/tca0.c 
	
${OBJECTDIR}/mcc_generated_files/timer/src/tcb1.o: mcc_generated_files/timer/src/tcb1.c  .generated_files/flags/default/5fe2bde15a4957765b33043f6b970aafb0c132b .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files/timer/src" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/timer/src/tcb1.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/timer/src/tcb1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O2 -Og -ffunction-sections -fdata-sections -fshort-enums -fcommon -funsigned-char -funsigned-bitfields -DF_CPU=16000000UL -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mconst-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/mcc_generated_files/timer/src/tcb1.o.d" -MT "${OBJECTDIR}/mcc_generated_files/timer/src/tcb1.o.d" -MT ${OBJECTDIR}/mcc_generated_files/timer/src/tcb1.o -o ${OBJECTDIR}/mcc_generated_files/timer/src/tcb1.o mcc_generated_files/timer/src/tcb1.c 
	
else
${OBJECTDIR}/mcc_generated_files/spi/src/spi0.o: mcc_generated_files/spi/src/spi0.c  .generated_files/flags/default/4637ef1c0e7958f713fb5b260de1071af5e47f53 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files/spi/src" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/spi/src/spi0.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/spi/src/spi0.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O2 -Og -ffunction-sections -fdata-sections -fshort-enums -fcommon -funsigned-char -funsigned-bitfields -DF_CPU=16000000UL -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mconst-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/mcc_generated_files/spi/src/spi0.o.d" -MT "${OBJECTDIR}/mcc_generated_files/spi/src/spi0.o.d" -MT ${OBJECTDIR}/mcc_generated_files/spi/src/spi0.o -o ${OBJECTDIR}/mcc_generated_files/spi/src/spi0.o mcc_generated_files/spi/src/spi0.c 
	
${OBJECTDIR}/mcc_generated_files/system/src/pins.o: mcc_generated_files/system/src/pins.c  .generated_files/flags/default/faf9bb9117834c2b1287ce7691d7987abdae21bb .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files/system/src" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/system/src/pins.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/system/src/pins.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O2 -Og -ffunction-sections -fdata-sections -fshort-enums -fcommon -funsigned-char -funsigned-bitfields -DF_CPU=16000000UL -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mconst-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/mcc_generated_files/system/src/pins.o.d" -MT "${OBJECTDIR}/mcc_generated_files/system/src/pins.o.d" -MT ${OBJECTDIR}/mcc_generated_files/system/src/pins.o -o ${OBJECTDIR}/mcc_generated_files/system/src/pins.o mcc_generated_files/system/src/pins.c 
	
${OBJECTDIR}/mcc_generated_files/system/src/interrupt.o: mcc_generated_files/system/src/interrupt.c  .generated_files/flags/default/17f5338bde394069138f4115337a9aa61674b35b .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files/system/src" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/system/src/interrupt.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/system/src/interrupt.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O2 -Og -ffunction-sections -fdata-sections -fshort-enums -fcommon -funsigned-char -funsigned-bitfields -DF_CPU=16000000UL -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mconst-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/mcc_generated_files/system/src/interrupt.o.d" -MT "${OBJECTDIR}/mcc_generated_files/system/src/interrupt.o.d" -MT ${OBJECTDIR}/mcc_generated_files/system/src/interrupt.o -o ${OBJECTDIR}/mcc_generated_files/system/src/interrupt.o mcc_generated_files/system/src/interrupt.c 
	
${OBJECTDIR}/mcc_generated_files/system/src/system.o: mcc_generated_files/system/src/system.c  .generated_files/flags/default/b873a1d6bad9870d582e4540b49a447b17cfaa12 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files/system/src" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/system/src/system.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/system/src/system.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O2 -Og -ffunction-sections -fdata-sections -fshort-enums -fcommon -funsigned-char -funsigned-bitfields -DF_CPU=16000000UL -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mconst-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/mcc_generated_files/system/src/system.o.d" -MT "${OBJECTDIR}/mcc_generated_files/system/src/system.o.d" -MT ${OBJECTDIR}/mcc_generated_files/system/src/system.o -o ${OBJECTDIR}/mcc_generated_files/system/src/system.o mcc_generated_files/system/src/system.c 
	
${OBJECTDIR}/mcc_generated_files/system/src/config_bits.o: mcc_generated_files/system/src/config_bits.c  .generated_files/flags/default/2b79af104f9697de49492f45ff22bc58f9c6dbc3 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files/system/src" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/system/src/config_bits.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/system/src/config_bits.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O2 -Og -ffunction-sections -fdata-sections -fshort-enums -fcommon -funsigned-char -funsigned-bitfields -DF_CPU=16000000UL -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mconst-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/mcc_generated_files/system/src/config_bits.o.d" -MT "${OBJECTDIR}/mcc_generated_files/system/src/config_bits.o.d" -MT ${OBJECTDIR}/mcc_generated_files/system/src/config_bits.o -o ${OBJECTDIR}/mcc_generated_files/system/src/config_bits.o mcc_generated_files/system/src/config_bits.c 
	
${OBJECTDIR}/mcc_generated_files/system/src/clock.o: mcc_generated_files/system/src/clock.c  .generated_files/flags/default/2564f2e223835ddcf6d9deeda7d8baf262cd7ec6 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files/system/src" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/system/src/clock.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/system/src/clock.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O2 -Og -ffunction-sections -fdata-sections -fshort-enums -fcommon -funsigned-char -funsigned-bitfields -DF_CPU=16000000UL -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mconst-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/mcc_generated_files/system/src/clock.o.d" -MT "${OBJECTDIR}/mcc_generated_files/system/src/clock.o.d" -MT ${OBJECTDIR}/mcc_generated_files/system/src/clock.o -o ${OBJECTDIR}/mcc_generated_files/system/src/clock.o mcc_generated_files/system/src/clock.c 
	
${OBJECTDIR}/mcc_generated_files/timer/src/tcb0.o: mcc_generated_files/timer/src/tcb0.c  .generated_files/flags/default/d9bf50d4b3523895f65316d94a28886f88e7d077 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files/timer/src" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/timer/src/tcb0.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/timer/src/tcb0.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O2 -Og -ffunction-sections -fdata-sections -fshort-enums -fcommon -funsigned-char -funsigned-bitfields -DF_CPU=16000000UL -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mconst-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/mcc_generated_files/timer/src/tcb0.o.d" -MT "${OBJECTDIR}/mcc_generated_files/timer/src/tcb0.o.d" -MT ${OBJECTDIR}/mcc_generated_files/timer/src/tcb0.o -o ${OBJECTDIR}/mcc_generated_files/timer/src/tcb0.o mcc_generated_files/timer/src/tcb0.c 
	
${OBJECTDIR}/mcc_generated_files/timer/src/rtc.o: mcc_generated_files/timer/src/rtc.c  .generated_files/flags/default/5dcbc9ebe5b674755922b3c5b986b16f6b3e5a55 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files/timer/src" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/timer/src/rtc.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/timer/src/rtc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O2 -Og -ffunction-sections -fdata-sections -fshort-enums -fcommon -funsigned-char -funsigned-bitfields -DF_CPU=16000000UL -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mconst-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/mcc_generated_files/timer/src/rtc.o.d" -MT "${OBJECTDIR}/mcc_generated_files/timer/src/rtc.o.d" -MT ${OBJECTDIR}/mcc_generated_files/timer/src/rtc.o -o ${OBJECTDIR}/mcc_generated_files/timer/src/rtc.o mcc_generated_files/timer/src/rtc.c 
	
${OBJECTDIR}/mcc_generated_files/uart/src/usart2.o: mcc_generated_files/uart/src/usart2.c  .generated_files/flags/default/91efeafc98fc48b91244cd20862bc5fa31b4ba98 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files/uart/src" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/uart/src/usart2.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/uart/src/usart2.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O2 -Og -ffunction-sections -fdata-sections -fshort-enums -fcommon -funsigned-char -funsigned-bitfields -DF_CPU=16000000UL -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mconst-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/mcc_generated_files/uart/src/usart2.o.d" -MT "${OBJECTDIR}/mcc_generated_files/uart/src/usart2.o.d" -MT ${OBJECTDIR}/mcc_generated_files/uart/src/usart2.o -o ${OBJECTDIR}/mcc_generated_files/uart/src/usart2.o mcc_generated_files/uart/src/usart2.c 
	
${OBJECTDIR}/mcc_generated_files/uart/src/usart1.o: mcc_generated_files/uart/src/usart1.c  .generated_files/flags/default/530fd1b5e5ee3ed0004e140026de8ae46cc5e82d .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files/uart/src" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/uart/src/usart1.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/uart/src/usart1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O2 -Og -ffunction-sections -fdata-sections -fshort-enums -fcommon -funsigned-char -funsigned-bitfields -DF_CPU=16000000UL -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mconst-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/mcc_generated_files/uart/src/usart1.o.d" -MT "${OBJECTDIR}/mcc_generated_files/uart/src/usart1.o.d" -MT ${OBJECTDIR}/mcc_generated_files/uart/src/usart1.o -o ${OBJECTDIR}/mcc_generated_files/uart/src/usart1.o mcc_generated_files/uart/src/usart1.c 
	
${OBJECTDIR}/main.o: main.c  .generated_files/flags/default/109867b89ff131a896a6a207c1d662eb6f01677 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O2 -Og -ffunction-sections -fdata-sections -fshort-enums -fcommon -funsigned-char -funsigned-bitfields -DF_CPU=16000000UL -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mconst-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/main.o.d" -MT "${OBJECTDIR}/main.o.d" -MT ${OBJECTDIR}/main.o -o ${OBJECTDIR}/main.o main.c 
	
${OBJECTDIR}/functions/modbus.o: functions/modbus.c  .generated_files/flags/default/920ea2a0a91083756e1842755c41196ab49d70f2 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/functions" 
	@${RM} ${OBJECTDIR}/functions/modbus.o.d 
	@${RM} ${OBJECTDIR}/functions/modbus.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O2 -Og -ffunction-sections -fdata-sections -fshort-enums -fcommon -funsigned-char -funsigned-bitfields -DF_CPU=16000000UL -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mconst-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/functions/modbus.o.d" -MT "${OBJECTDIR}/functions/modbus.o.d" -MT ${OBJECTDIR}/functions/modbus.o -o ${OBJECTDIR}/functions/modbus.o functions/modbus.c 
	
${OBJECTDIR}/functions/debug_uart2.o: functions/debug_uart2.c  .generated_files/flags/default/530c8f36c8e565f53f09e3ff41d5b221a057e96 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/functions" 
	@${RM} ${OBJECTDIR}/functions/debug_uart2.o.d 
	@${RM} ${OBJECTDIR}/functions/debug_uart2.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O2 -Og -ffunction-sections -fdata-sections -fshort-enums -fcommon -funsigned-char -funsigned-bitfields -DF_CPU=16000000UL -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mconst-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/functions/debug_uart2.o.d" -MT "${OBJECTDIR}/functions/debug_uart2.o.d" -MT ${OBJECTDIR}/functions/debug_uart2.o -o ${OBJECTDIR}/functions/debug_uart2.o functions/debug_uart2.c 
	
${OBJECTDIR}/functions/temp_sensors.o: functions/temp_sensors.c  .generated_files/flags/default/690c4fe0d153363a62e2426dfc2e6e9656072093 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/functions" 
	@${RM} ${OBJECTDIR}/functions/temp_sensors.o.d 
	@${RM} ${OBJECTDIR}/functions/temp_sensors.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O2 -Og -ffunction-sections -fdata-sections -fshort-enums -fcommon -funsigned-char -funsigned-bitfields -DF_CPU=16000000UL -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mconst-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/functions/temp_sensors.o.d" -MT "${OBJECTDIR}/functions/temp_sensors.o.d" -MT ${OBJECTDIR}/functions/temp_sensors.o -o ${OBJECTDIR}/functions/temp_sensors.o functions/temp_sensors.c 
	
${OBJECTDIR}/functions/system_registers.o: functions/system_registers.c  .generated_files/flags/default/35c367fffdf192a76d8717960b7faf24146d16c1 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/functions" 
	@${RM} ${OBJECTDIR}/functions/system_registers.o.d 
	@${RM} ${OBJECTDIR}/functions/system_registers.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O2 -Og -ffunction-sections -fdata-sections -fshort-enums -fcommon -funsigned-char -funsigned-bitfields -DF_CPU=16000000UL -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mconst-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/functions/system_registers.o.d" -MT "${OBJECTDIR}/functions/system_registers.o.d" -MT ${OBJECTDIR}/functions/system_registers.o -o ${OBJECTDIR}/functions/system_registers.o functions/system_registers.c 
	
${OBJECTDIR}/DS.o: DS.c  .generated_files/flags/default/1ef0d80458c853972b62c5e1b303f6c3fa877a6e .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/DS.o.d 
	@${RM} ${OBJECTDIR}/DS.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O2 -Og -ffunction-sections -fdata-sections -fshort-enums -fcommon -funsigned-char -funsigned-bitfields -DF_CPU=16000000UL -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mconst-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/DS.o.d" -MT "${OBJECTDIR}/DS.o.d" -MT ${OBJECTDIR}/DS.o -o ${OBJECTDIR}/DS.o DS.c 
	
${OBJECTDIR}/dht11.o: dht11.c  .generated_files/flags/default/68a3cda5c4265b4643c75af6d0bfd6f9171d1c6c .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/dht11.o.d 
	@${RM} ${OBJECTDIR}/dht11.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O2 -Og -ffunction-sections -fdata-sections -fshort-enums -fcommon -funsigned-char -funsigned-bitfields -DF_CPU=16000000UL -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mconst-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/dht11.o.d" -MT "${OBJECTDIR}/dht11.o.d" -MT ${OBJECTDIR}/dht11.o -o ${OBJECTDIR}/dht11.o dht11.c 
	
${OBJECTDIR}/mcc_generated_files/timer/src/tcb2.o: mcc_generated_files/timer/src/tcb2.c  .generated_files/flags/default/f90218773c899f800154c254e56d712f21a9dd15 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files/timer/src" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/timer/src/tcb2.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/timer/src/tcb2.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O2 -Og -ffunction-sections -fdata-sections -fshort-enums -fcommon -funsigned-char -funsigned-bitfields -DF_CPU=16000000UL -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mconst-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/mcc_generated_files/timer/src/tcb2.o.d" -MT "${OBJECTDIR}/mcc_generated_files/timer/src/tcb2.o.d" -MT ${OBJECTDIR}/mcc_generated_files/timer/src/tcb2.o -o ${OBJECTDIR}/mcc_generated_files/timer/src/tcb2.o mcc_generated_files/timer/src/tcb2.c 
	
${OBJECTDIR}/mcc_generated_files/timer/src/tca0.o: mcc_generated_files/timer/src/tca0.c  .generated_files/flags/default/613379d77327e407691b5f1a487ad4610e82e06d .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files/timer/src" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/timer/src/tca0.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/timer/src/tca0.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O2 -Og -ffunction-sections -fdata-sections -fshort-enums -fcommon -funsigned-char -funsigned-bitfields -DF_CPU=16000000UL -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mconst-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/mcc_generated_files/timer/src/tca0.o.d" -MT "${OBJECTDIR}/mcc_generated_files/timer/src/tca0.o.d" -MT ${OBJECTDIR}/mcc_generated_files/timer/src/tca0.o -o ${OBJECTDIR}/mcc_generated_files/timer/src/tca0.o mcc_generated_files/timer/src/tca0.c 
	
${OBJECTDIR}/mcc_generated_files/timer/src/tcb1.o: mcc_generated_files/timer/src/tcb1.c  .generated_files/flags/default/45eed06aa2e163cdafd84ad38820591cd3e9d5c5 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files/timer/src" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/timer/src/tcb1.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/timer/src/tcb1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O2 -Og -ffunction-sections -fdata-sections -fshort-enums -fcommon -funsigned-char -funsigned-bitfields -DF_CPU=16000000UL -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mconst-data-in-progmem -mconst-data-in-config-mapped-progmem     -MD -MP -MF "${OBJECTDIR}/mcc_generated_files/timer/src/tcb1.o.d" -MT "${OBJECTDIR}/mcc_generated_files/timer/src/tcb1.o.d" -MT ${OBJECTDIR}/mcc_generated_files/timer/src/tcb1.o -o ${OBJECTDIR}/mcc_generated_files/timer/src/tcb1.o mcc_generated_files/timer/src/tcb1.c 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/mcc_generated_files/system/src/protected_io.o: mcc_generated_files/system/src/protected_io.S  .generated_files/flags/default/8175a0603b30452ab398560c00a76ef1b6873d18 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files/system/src" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/system/src/protected_io.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/system/src/protected_io.o 
	${MP_CC} -c $(MP_EXTRA_AS_PRE) -mcpu=$(MP_PROCESSOR_OPTION)  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x assembler-with-cpp -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O2 -Og -ffunction-sections -fdata-sections -fshort-enums -fcommon -funsigned-char -funsigned-bitfields -DF_CPU=16000000UL -Wall -DXPRJ_default=$(CND_CONF)  -gdwarf-3 -mconst-data-in-progmem -mconst-data-in-config-mapped-progmem -Wa,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1   -MD -MP -MF "${OBJECTDIR}/mcc_generated_files/system/src/protected_io.o.d" -MT "${OBJECTDIR}/mcc_generated_files/system/src/protected_io.o.d" -MT ${OBJECTDIR}/mcc_generated_files/system/src/protected_io.o -o ${OBJECTDIR}/mcc_generated_files/system/src/protected_io.o  mcc_generated_files/system/src/protected_io.S 
	
else
${OBJECTDIR}/mcc_generated_files/system/src/protected_io.o: mcc_generated_files/system/src/protected_io.S  .generated_files/flags/default/d1cac2b0b1ce4fbb50b54679cef10688e797a7b .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files/system/src" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/system/src/protected_io.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/system/src/protected_io.o 
	${MP_CC} -c $(MP_EXTRA_AS_PRE) -mcpu=$(MP_PROCESSOR_OPTION)  -x assembler-with-cpp -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O2 -Og -ffunction-sections -fdata-sections -fshort-enums -fcommon -funsigned-char -funsigned-bitfields -DF_CPU=16000000UL -Wall -DXPRJ_default=$(CND_CONF)  -gdwarf-3 -mconst-data-in-progmem -mconst-data-in-config-mapped-progmem -Wa,--defsym=__MPLAB_BUILD=1   -MD -MP -MF "${OBJECTDIR}/mcc_generated_files/system/src/protected_io.o.d" -MT "${OBJECTDIR}/mcc_generated_files/system/src/protected_io.o.d" -MT ${OBJECTDIR}/mcc_generated_files/system/src/protected_io.o -o ${OBJECTDIR}/mcc_generated_files/system/src/protected_io.o  mcc_generated_files/system/src/protected_io.S 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${DISTDIR}/Sensor_Controller.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -Wl,-Map=${DISTDIR}/Sensor_Controller.X.${IMAGE_TYPE}.map  -D__DEBUG=1  -DXPRJ_default=$(CND_CONF)  -Wl,--defsym=__MPLAB_BUILD=1   -mdfp="${DFP_DIR}/xc8"   -gdwarf-2 -Wl,--gc-sections -O2 -Og -ffunction-sections -fdata-sections -fshort-enums -fcommon -funsigned-char -funsigned-bitfields -DF_CPU=16000000UL -Wall -gdwarf-3 -mconst-data-in-progmem -mconst-data-in-config-mapped-progmem     $(COMPARISON_BUILD) -Wl,--memorysummary,${DISTDIR}/memoryfile.xml -o ${DISTDIR}/Sensor_Controller.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  -o ${DISTDIR}/Sensor_Controller.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -Wl,--start-group  -Wl,-lm -Wl,--end-group  -Wl,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1
	@${RM} ${DISTDIR}/Sensor_Controller.X.${IMAGE_TYPE}.hex 
	
	
else
${DISTDIR}/Sensor_Controller.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -Wl,-Map=${DISTDIR}/Sensor_Controller.X.${IMAGE_TYPE}.map  -DXPRJ_default=$(CND_CONF)  -Wl,--defsym=__MPLAB_BUILD=1   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O2 -Og -ffunction-sections -fdata-sections -fshort-enums -fcommon -funsigned-char -funsigned-bitfields -DF_CPU=16000000UL -Wall -gdwarf-3 -mconst-data-in-progmem -mconst-data-in-config-mapped-progmem     $(COMPARISON_BUILD) -Wl,--memorysummary,${DISTDIR}/memoryfile.xml -o ${DISTDIR}/Sensor_Controller.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  -o ${DISTDIR}/Sensor_Controller.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -Wl,--start-group  -Wl,-lm -Wl,--end-group 
	${MP_CC_DIR}\\avr-objcopy -O ihex "${DISTDIR}/Sensor_Controller.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}" "${DISTDIR}/Sensor_Controller.X.${IMAGE_TYPE}.hex"
	
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${OBJECTDIR}
	${RM} -r ${DISTDIR}

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(wildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
