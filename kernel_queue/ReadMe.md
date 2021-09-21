


# File 요약

### Kernel Version : 4.4.0.osta

### call_my_queue.c 

* /usr/src/call_my_queue.c 

* User application 소스코드 파일

### call_my_queue

* /usr/src/call_my_queue

* call_my_queue.c 컴파일된 실행 파일

### Makefile

* /usr/src/linux-4.4/kernel/Makefile

* 커널 컴파일 위한 파일 

### my_queue_syscall.c

* /usr/src/linux-4.4/kernel/my_queue_syscall.c

* 커널 모드에서 호출할 시스템콜 함수가 담긴 파일

### syscall_64.tbl

* /usr/src/linux-4.4/arch/x86/entry/syscalls/syscall_64.tbl

* 새로 추가한 시스템콜 고유 번호가 기재된 시스템콜 테이블 파일

### syscalls.h

* /usr/src/linux-4.4/include/linux/syscalls.h

* 새로 추가한 시스템콜 함수 prototype 정의된 파일

# Kernel 수정

## 1.	개발환경
<pre>
Host PC 
OS : Windows 10 64bit

Guest PC 
OS :  Ubuntu 16.04 LTS (Virtual Box)
Disk : 10GB
RAM : 2048MB
CPU processor : 2
Kernel Version : 4.4.0.osta
</pre>

## 2.	1차 과제에서의 리눅스 Trap, 시스템 콜 개념 및 과정 정리

이번 운영체제 1차 과제에서는 kernel mode에서 동작하는 시스템콜과 user mode에서 동작하는 application 각각을 직접 만들어 함께 실행해보았다. 
먼저 User mode에 있는 user application (call_my_queue) 파일에서 직접 만들어 삽입한 시스템콜인 my_queue _syscall을 호출한다. 
시스템콜은 user 모드에서는 실행되지 않아서 커널 모드에 요청 후 해당 모드에서 직접 실행되어야 한다. 시스템콜을 커널 모드로 실행 요청하기 위해서는 syscall ( ) 함수를 이용하며, 
인자로 시스템콜 테이블에서 내가 정한 함수의 테이블 고유 번호도 함께 파라미터로 전달해준다. 전달된 테이블 넘버는 CPU의 레지스터에 저장되며, 
user mode에서 kernel mode로 넘어가는 시스템콜 인터페이스에서 Trap이 발생한다. Trap은 소프트웨어 인터럽트라고도 하며, user가 원하는 시스템콜을 커널 모드에서 실행할 준비를 한다. 
커널 모드에서는 이전에 레지스터에 저장된 테이블 넘버를 테이블(syscall_64.tbl)에서 찾아 user 대신 수행하게 된다. 시스템콜이 모두 수행되면 시스템콜 결과를 레지스터로 보낸다. 
시스템콜 인터페이스를 통해 user mode로 돌아오게 되면 방금 레지스터로 보낸 시스템콜 결과를 이용해 다음 연산 및 명령을 진행할 수 있다.


## 3.	수정 및 작성한 소스코드 설명


### syscall_64.tbl

 새로 추가한 시스템콜(sys_my_enqueue, sys_my_dequeue)의 고유 번호를 저장하였다. 각 함수의 고유 번호는 326, 327로 지정하였다.


### syscalls.h

시스템콜 테이블에서 명시한 함수들의 prototype을 기록하였다. sys_my_enqueue의 경우 int형 값이 queue에 추가되어야 하므로 int형 인자를 넣었으며, 반환되는 값은 없으므로 void를 썼다. sys_my_dequeue의 경우 전달되는 인자는 없지만 dequeue 되어서 반환되는 값은 user mode에서 알 수 있어야 하므로 반환 자료형은 queue의 각 값들과 동일한 자료형인 int를 썼다. 두 함수 모두 앞에 asmlinkage를 썼으며 어셈블리 코드로 구성된 trap에서도 C 함수를 호출하게 만들기 위해서 asmlinkage를 써줬다. 

 
### call_my_queue.c , call_my_queue

시스템콜 함수를 호출하는 user application 소스 코드 파일과 컴파일한 파일이다. 시스템콜 테이블에서 지정한 326, 327을 syscall ( ) 인자로 전달하며 enqueue, dequeue를 수행한다. 
 1~3을 enqueue하기 위해 시스템콜 sys_my_enqueue 함수 넘버인 326과 삽입할 int형 수를 반복문을 이용해 전달하였다. 이어서 기존에 있는 2라는 숫자를 한 번 더 삽입 시도해 봄으로써 queue에 이미 있는 숫자는 더 이상 들어갈 수 없음을 보였다. 반복문을 이용해 queue에 있는 값들을 FIFO 형식으로 dequeue되게 만들었다. 그리고 queue가 비어 있지 않다는 전제 하에 dequeue 된 값을 출력되게 하였다. 


### Makefile

$sudo make 시 새로 추가한 시스템콜 함수까지 올바르게 컴파일 될 수 있도록 my_queue_syscall.o를 명시해줬다.

### my_queue_syscall.c

 이 파일에는 Trap에 의해 호출되는 시스템콜 함수인 sys_my_enqueue, sys_my_dequeue의 원형이 있다. 두 함수 모두 array queue에 값을 삽입, 제거할 수 있는 구조로 구성하였으며, queue에 int형 값 최대 100개 삽입할 수 있게 만들었다.   sys_my_enqueue 함수에서는 queue에 저장된 값들의 개수가 100을 넘거나, 추가하려는 값이 이미 queue에 있다는 것이 반복문으로 확인되면 함수는 종료된다. 공간이 남아있고 중복된 값이 없는 경우, r 변수를 0부터 1씩 증가시키며 queue의 r번째 인덱스 자리에 값을 저장한다. 값 1개당 r이 1씩 증가한다. 그리고 현재 queue에 저장된 모든 값들을 커널 모드에서 출력한다.
 sys_my_dequeue 함수에서는 queue가 비게 되면 -2를 반환하게 된다. 원래 -1로 설정했지만 커널 모드에서 OS가 시스템콜 함수 호출 실패 시, user application에 -1을 반환하는 점 때문에 디버깅에 조금 혼란이 왔었다. 이를 쉽게 구분해주기 위해 -2로 수정했다. 그리고 sys_my_dequeue 함수는 Queue의 제일 앞에 위치한 값을 dequeue하며 뒤의 인덱스에 위치한 값들을 반복문으로 한 칸 씩 앞으로 당겨준다. 그리고 현재 queue에 저장된 모든 값들을 커널 모드에서 출력하며 user application으로 dequeue된 값을 반환한다.  


## 4.	실행 결과 스크린샷

### User application 컴파일, 실행 결과

 <p align="center">
  <img width="800" height="200" src="https://user-images.githubusercontent.com/80872528/134100981-26481fc7-4882-4445-905d-ea34d0381336.png">
</p>


### Kernel 시스템콜 실행 결과

 <p align="center">
  <img width="800" height="500" src="https://user-images.githubusercontent.com/80872528/134101027-43e1047e-140e-4dc1-b6e0-d484a2b85657.png">
</p>
  


## 5.	발생한 문제점 및 해결 방법

### 커널 설정 변경 미확인

linux-4.4 디렉토리에서 시스템콜 관련 파일들을 수정, 추가 후 /usr/src에 있는 call_my_queue 파일을 실행하니 모든 enqueue, deuque 결과로 -1이 나와서 디버깅을 하게 되었다. 4.4 디렉토리 내부 파일들을 수정하고 user application도 수정해보았지만 변화가 없어서 운영체제 설치 자료부터 다시 검토해보았다. 
0차 과제에서 4.4.0.osta 커널 설치된 것 확인하고 종료를 했는데 커널 부팅 설정 변경을 잘 못 해서 1차 과제를 하기 위해 부팅한 이후로 계속 4.15 커널로 진행을 하고 있었다. 커널 부팅 설정 시 GRUB_DEFAULT를 “1>3”이 아닌 “1>8” (4.4.0.osta recovery mode) 로 수정해주니 커널이 정상적으로 변경된 것을 확인할 수 있었다. 저번 0차 과제 진행 중 재부팅 시, “1>3” 임에도 4.4 커널이 적용되었는데 어떤 이유로 “1>3” 자리에 있던 4.4가 사라졌는지는 아직 의문이며, 혹시 디스크 용량 해결하려 /usr/src 내부의 여러 4.15 커널 모드 중 한 개를 지운 것이 그 이유인가 추측하고 있다.
 
### 커널 컴파일 된 것이 적용되지 않음

위에서 발생한 커널 문제를 해결하며 자연스럽게 재부팅을 하니 user application과 추가한 시스템콜에서 의도한대로 출력 결과가 나오게 되었다. 하지만 여기에 queue가 모두 찼을 경우, 비었을 경우를 추가해주고 싶어 시스템콜 함수 코드를 수정 후 커널 컴파일 해보았지만 출력 결과는 전과 동일했다. 

 잘 풀리지 않아 시스템을 종료하고 다른 공부를 하다 돌아와서 재부팅 하니 컴파일 된 커널이 정상적으로 동작하는 것을 볼 수 있었다. 새로 컴파일 한 커널을 적용하려면 재부팅 해야 된다는 내용이 우분투 커널 설치 자료에 있었는데 잊고 있었다. 정말 기본적인 것인데 다시 한 번 되짚어 보고 머릿속에 새길 수 있었다.
