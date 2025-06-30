# TCP-IP-Socket-Programming
IoT 개발자 TCP/IP 소켓프로그래밍 리포지토리


## 1일차 
1. VMware 설치
    - [vmware](https://www.vmware.com/)
2. Ubuntu 설치
    - [Ubuntu 설치링크](https://ubuntu.com/download/desktop)
3. PuTTY 설치
    - [PuTTY 설치링크](https://www.putty.org/)

Ctrl + Alt + T 터미널 열기 단축키
```
sudo apt install net-tools // 네트워크 관련 도구 설치
sudo apt install openssh-server // ssh 서버(원격 접속 기능) 설치
ifconfig
> inet 102.xxx.xxx.xxx
```
```putty
> sudo apt update
> sudo apt upgrade
```

nano 편집기 설정
```
> sudo nano /etc/nanorc
set autoindent
set linenumbers
set tabsize 3
```

gcc 컴파일러 설치
```
> sudo apt install gcc
```
### 리눅스 기본 명령어 정리
- `uname- a` : 정보확인
- `cd` : 폴더 이동
- `ls` : 폴더 목록 보여줌
- `pwd` : 현재디렉토리 위치
- `cd ..` : 상위폴더로 이동
- `mkdir` : 디렉토리 생성
- `touch` : 빈 파일 생성
- `cat` : 화면 출력
- `cp` : 복사
- `clear` : 터미널 기록 삭제
- `rm -fr` : 파일/폴더 삭제
- `sudo nano /etc/nanorc` : nano 편집기 설정
- `gcc {파일명}` : 컴파일
	- -o 실행파일 이름 설정가능 
	- `gcc main.c -o {실행파일명}` (기본값 a.out)
- `./a.out` : 실행파일 실행
- `chmod` : 파일권한 변경

네트워크 프로그램은 일반적으로 클라이언트-서버 모델로 작성한다.
클라이언트-서버 : 두 프로그램이 상호작용하는 방식
- 클라이언트 : 서비스를 요청하는 쪽
- 서버 : 클라이언트가 요청한 서비스를 처리하는 쪽

### 소켓(Socket)
네트워크 통신의 끝단(endpoint)
클라이언트와 서버는 **소켓을 통해 연결**되어 데이터를 주고 받는다

#### 서버 소켓 함수 호출 순서
1. socket() : 소켓 생성
2. bind() : 소켓 연결(주소 할당 - IP주소와 포트 소켓에 할당)
3. listen() : 연결 요청 대기(서버)
4. accept() : 클라 연결 허용(서버) & 새로운 소켓 반환: 반환된 소켓으로 실제 데이터 송수신에 사용
5. recv()/send() : 데이터 송/수신
6. close() : 소켓 종료

#### 클라이언트 소켓 함수 호출 순서
1. socket() : 소켓 생성
2. connect() : 서버에 연결 요청
3. send()/recv() : 데이터 송수신
4. close() : 소켓 종료

#### open()
```
int open(const char* name, int flags);
``` 

**open() 함수에서 사용하는 주요 플래그**
- #include<fcntl.h> : 헤더 추가필요
- `O_RDONLY`: 읽기 전용으로 파일 열기
- `O_WRONLY`: 쓰기 전용으로 파일 열기
- `O_RDWR`: 읽기 및 쓰기 모두 가능하게 파일 열기
- `O_CREAT`: 파일이 없으면 새로 생성 (권한 필요)
- `O_TRUNC`: 파일을 열 때 기존 내용 제거
- `O_APPEND`: 파일 끝에 데이터를 추가 (이어쓰기 모드)
**플래그는 | 연산자로 조합하여 사용함**
- 예: O_WRONLY | O_CREAT | O_TRUNC

#### read()
```
#include<unistd>

ssize_t read(int fd, void* buf, size_t count);
```
- fd: 읽고자 하는 파일 디스크립터 번호 (open() 또는 socket() 등을 통해 얻음)
- buf: 읽은 데이터를 저장할 메모리 주소 (버퍼)
- count: 최대 읽을 바이트 수 (버퍼 크기)
- 참고
  - ssize_t는 음수도 표현 가능한 정수형으로, 오류(-1) 처리를 위해 사용됨 (size_t : 양수만)
  - 파일 디스크립터(fd)는 리눅스에서 열린 파일을 식별하는 정수 값
#### write()
```
#include<unistd>

ssize_t write(int fd, const void* buf, size_t nbytes);
```
### 엔디안(Endian)
여러 바이트로 구성된 데이터를 메모리에 저장하거나 전송할 때, 바이트를 어떤 순서로 배열할 것인가를 결정하는 방식

#### 리틀 엔디안과 빅 엔디안
1. 빅 엔디안(Big Endian)
가장 앞 바이트에 상위 바이트를 저장

2. 리틀 엔디안(Little Endian)
가장 앞 바이트에 하위 바이트를 저장

예시) 16진수 숫자 0x12345678을 4바이트로 저장한다고 가정하면
- 빅 엔디안 : 12 34 56 78
- 리틀 엔디안 78 56 34 12
- [참고 코드](./chapter1/endian1.c)

TCP/IP 소켓 통신은 서로 다른 시스템 간의 통신이기 때문에 
A 시스템 : 리틀 엔디안, B 시스템 : 빅 엔디안 의 경우와 같이 다를 수 있다.
그러므로 통신할 때는 엔디안을 통일 시켜줘야 한다.

TCP/IP 프로토콜에서는 항상 **빅 엔디안**을 사용한다.
C/C++ 에서는 `htons`, `htonl`, `ntohs`, `ntohl` 같은 함수들을 사용해 엔디안 변환을 해준다.
- 시스템 바이트 순서 → 네트워크 바이트 순서
- 시스템 바이트 순서 → 네트워크 바이트 순서 

| 용어               | 설명                           |
| ---------------- | ---------------------------- |
| Big Endian       | 상위 바이트부터 저장 (네트워크 표준)        |
| Little Endian    | 하위 바이트부터 저장 (x86 시스템에서 일반적)  |
| htons()          | host to network short (2바이트) |
| htonl()          | host to network long (4바이트)  |
| ntohs(), ntohl() | 네트워크 → 호스트 변환                |
