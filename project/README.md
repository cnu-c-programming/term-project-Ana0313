# Mini Student Shell

- c언어로 작성된 학생 데이터 관리 프로그램입니다.
- csv 파일을 통한 데이터 저장 및 불러오기를 지원합니다.
- 권한에 따라 관리자(Admin)모드와 클라이언트(Client)모드로 나뉘어 실행됩니다.

## 파일 구조
```
project/
├── main.c       
├── student.h     
├── student.c  
├── file_io.h
├── file_io.c      
├── command.h    
├── command.c   
├── Makefile
├── README.md 
├── commands.txt
└── students.csv
```

## 주요 기능
- 연결 리스트를 활용한 빠른 학생 데이터 추가/삭제/검색/수정.
- CSV 파일과 연동하여 영구적인 데이터 보관 및 불러오기.
- 권한 분리: Admin Mode(데이터 수정 및 저장 등 모든 권한 포함.), Client Mode(데이터 조회만 가능한 읽기 전용 권한.)
- 사용자가 직접 명령어를 타이핑하는 대화형 모드 외에도, 명령어들이 적힌 텍스트 파일을 읽어 순차적으로 자동 실행하는 기능 지원.
- 관리자 모드에서 저장되지 않은 변경 사항이 있을 경우, 종료 전 경고 메시지 출력.

##  빌드 방법(컴파일)
이 프로젝트는 Makefile을 포함하고 있어, 터미널에서 간단한 명령어로 빌드할 수 있습니다.
```
make all        # 모든 프로그램 빌드
make admin      # admin_shell만 빌드
make client     # client_shell만 빌드
make clean      # 빌드 파일 삭제
```

## 실행 방법
make 한 뒤에 터미널에 직접 명령어를 입력하여 프로그램을 조작합니다.
```
# Interactive Mode
./admin_shell students.csv     # 관리자 모드로 실행
./client_shell students.csv    # 클라이언트 모드로 실행

# 명령어 파일 실행
./admin_shell -f commands.txt students.csv   # 관리자 모드에서 명령어 파일 실행
./client_shell -f commands.txt students.csv  # 클라이언트 모드에서 명령어 파일 실행
```
- CSV 파일이 없으면 빈 목록으로 시작합니다.
- 명령어 텍스트 파일에서 한 줄에 명령어는 하나여야 합니다.

## CSV 파일 기본 형식
```
id,name,score
1,Alice,90
2,Bob,85
3,Charlie,95
```
- id: 양수 정수여야 합니다.
- name: 비어 있을 수 없습니다. 쉽표를 사용할 수 없습니다.
- score: 0 이상 100 이하의 정수여야 합니다.
- 파일은 일반 텍스트 CSV 형식으로 저장해야 합니다.

## 명령어 텍스트 파일 기본 형식
```
list
add 4 David 88
update 99 70
find 4
save
exit
```
- #으로 시작하는 줄은 주석으로 무시
- 명령어 실행 시 [command file:N] 형태로 줄 번호 출력
- 오류 발생 시 해당 줄 건너뛰고 다음 명령어 계속 실행

## 명령어 목록
| 명령어 | 형식 | 설명 | 사용 가능 모드 |
|-------|-----------------|------|----------------|
| add | `add <id> <name> <score>` | 새로운 학생 데이터 추가 | admin |
| delete | `delete <id>` | 특정 id를 가진 학생을 삭제 | admin |
| update | `update <id> <score>` | 특정 id를 가진 학생의 점수를 수정 | admin |
| save | `save` | 현재까지의 변경 사항을 CSV 파일에 저장 | admin |
| reload | `reload` | CSV 파일로부터 데이터를 다시 불러옴 | admin, client |
| find | `find <id>` | 특정 id를 가진 학생의 상세 정보 출력 | admin, client |
| list | `list` | 전체 학생의 목록 출력 | admin, client |
| stats | `stats` | 총원, 평균 점수, 최고점, 최저점을 출력 | admin, client |
| clear | `clear` | 터미널 화면을 깨끗하게 지움 | admin, client |
| help | `help` | 사용 가능한 명령어 목록과 설명 출력 | admin, client |
| exit | `exit` | 프로그램 종료(미저장 시 경고 발생) | admin, client |
