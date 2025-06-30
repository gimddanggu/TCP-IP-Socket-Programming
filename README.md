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
> sudo apt install net-tools
> sudo apt install openssh-server
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
- uname- a : 정보확인
- cd : 폴더 이동
- ls : 폴더 목록 보여줌
- pwd : 현재디렉토리 위치
- cd .. : 상위폴더로 이동
- mkdir : 디렉토리 생성
- touch : 빈 파일 생성
- cat : 화면 출력
- cp : 복사
- clear : 터미널 기록 삭제
- rm -fr : 파일/폴더 삭제
- sudo nano /etc/nanorc : nano 편집기 설정
- gcc {파일명} : 컴파일
	- -o 실행파일 이름 설정가능 
	- gcc main.c -o {실행파일명} (기본값 a.out)
- ./a.out : 실행파일 실행
- chmod : 파일권한 변경

네트워크 프로그램은 일반적으로 클라이언트-서버 모델로 작성한다.
클라이언트-서버 : 두 프로그램이 상호작용하는 방식
- 클라이언트 : 서비스를 요청하는 쪽
- 서버 : 클라이언트가 요청한 서비스를 처리하는 쪽

### 리틀엔디안과 빅엔디안 (저녁에 정리)
