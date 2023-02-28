# Single_Cycle_MIPS_Emulator
MIPS의 text(code) segment에 존재하는 binary instruction을 실행할 수 있는 에뮬레이터

instruction들을 메모리에 저장하고, 메모리에 저장되어 있는 binary instruction들 중 PC값에 저장되어 있는 binary instruction을 읽어 이를 single cycle architecture의 process에 맞춰 IF, ID, EXE, MEM, WB 과정에 따라 instruction을 실행한다.

## Execution Process Summary
1.	bin파일의 경로를 받아 이를 읽어 instruction들을 차례대로 memory에 저장한다
2.	PC값에 해당하는 instruction을 fetch, decode한다.
3.	decode된 instruction의 이름(ADD, ADDU, etc)에 따라 해당 instruction에 맞는 control signal들을 생성한다. 
4.	control signal의 제어에 따라 EXE, MEM, WB를 수행한 후 PC 값을 변경한다
5.	변경된 PC 값이 0xFFFFFFFF(종료상태)가 아닐 경우 2번 작업으로 돌아가고, 종료상태일 경우 프로그램의 결과 값에 해당하는 v0와 v2을 출력한다.
