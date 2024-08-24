# get_next_line

- 이 과제는 get_next_line 이라는 과제명처럼 File Descriptor에서 문자를 개행을 만나기 전까지 계속 읽다가 개행을 만나면 읽는 것을 멈추고 개행을 포함한 문자열 1줄을 반환함
- 만약 개행없이 파일의 문자열이 EOF로 끝난다면 모든 문자열을 개행없이 반환함

## 학습 내용

- static 변수 : 변수의 값이 함수가 끝나도 프로그램을 종료하기 전까지는 값을 기억하고 있게 만들어줌
- read(fd, line, BUFFER_SIZE) : fd에 쓰여진 문자를 BUFFER_SIZE 만큼 읽은 후 주소의 형태로 line에 할당함 (문자열 포인터)
- read 함수의 반환 형식은 ssize_t(=long)이며 읽기를 성공한 문자의 개수를 반환함. 특이 사항으로 파일을 EOF까지 읽었음에도 read 함수를 계속 호출하게 되면 0값을 반환함 (Error의 경우 -1)
- free로 메모리 할당을 해제해도 포인터 변수에 들어있던 주소의 값은 0(NULL 포인터)으로 돌아가지 않음. 주소값을 0으로 바꿔주기 위해서는 별도로 0값을 할당해야 함 (*ptr = 0)
- 메모리 할당이 해제되어있는 주소를 계속 가리키는 포인터를 Dangling Pointer라 함
- 이중 연결 리스트에 대하여 알게 되었음

## 함수 설명 (Mandatory Part)

### char *get_next_line(int fd)

- fd를 인자값으로 받고 fd에서 문자열을 읽어 한줄을 반환하는 함수
- static char *save : static 변수를 사용하여 read 함수로 읽고 아직 출력되지 않은 문자열이 함수가 종료되었어도 프로그램이 종료되기 전까지는 기억할 수 있게 함
- new[BUFFER_SIZE + 1] : 새로운 문자열 한줄의 정보를 저장하는 배열 사용 (BUFFER_SIZE가 월등히 크면 문제가 발생할 수 있는 방식임)
- fd가 0 미만인 값과 BUFFER_SIZE가 0 이하인 경우는 예외처리

### void read_string(int fd, char **save, char *line, ssize_t *read_result)

- BUFFER_SIZE 만큼 문자열을 읽어서 line에 먼저 저장하고 이전 문자열이 있다면 strjoin으로 이어주고 없다면 strdup로 새로운 문자열을 생성해서 save 변수에 저장하는 함수
- strjoin 시 이전 문자열에 대한 free를 반드시 해주어야 함
- strdup, strjoin이 실패했을 때 save를 free 및 함수 종료
- 읽은 문자열(line)에 개행이 있거나 fd의 EOF까지 읽어서 read 함수의 반환값이 0이라면 read 함수를 더 호출하지 않고 read_string 함수를 종료함
- 읽은 문자열(line)에 개행이 없다면 반복문 조건에 따라 계속해서 read 진행

### void input_string(char **save, char **line)

- 읽은 문자열의 주소값이 저장된 save에서 개행 또는 0까지의 문자열 길이 값을 측정하여 메모리 동적할당 후 line 변수에 저장하는 함수
- 매개변수가 이중 포인터인 이유는 메모리 동적할당이 실패했을 때 save의 주소값에 해당하는 메모리의 동적할당을 해제하고 주소값을 0으로 바꾸기 위해서, 그리고 line에 할당한 메모리 주소값이 별도의 return 없이 유지되게 하기위해서 포인터 변수의 주소값으로 넘겨준 것이다.

### void next_string(char **save, char **line)

- get_next_line의 출력값으로 정해진 line의 길이를 측정하고 save에서 line의 문자 성분을 제외하고 새로운 문자열을 다시 만드는 함수
- 이 함수도 마찬가지로 별도의 리턴없이 구현하고 싶어서 2중 포인터를 사용함

### void free_memory(char **mem)

- 메모리의 동적 할당을 해제하고 메모리의 주소가 담긴 포인터의 변수를 0으로 되돌리는 함수
- if 조건으로 널 포인터를 해제하는 경우를 방지함 (*mem ≠ 0)

## 함수 설명 (Bonus Part)

- 1개의 fd만 핸들링하는 것이 아니라 여러 개의 fd를 핸들링할 수 있게 만드는 것이 보너스의 목표
- static 변수를 하나만 사용하는 것도 보너스였는데 mandatory에서 이미 해버림
- 여러 개의 fd를 읽을 수 있어야해서 이중 연결 리스트라는 구조를 사용함
    - 이중 연결 리스트 : 일반적인 연결 리스트는 구조체의 멤버 변수로 자료와 다음 노드의 주소값을 가지는데 이중 연결 리스트는 여기에 ‘이전 노드의 주소’를 저장하는 멤버 변수 하나가 추가되는 형식임
 
      <img width="604" alt="Screen Shot 2023-03-03 at 2 50 53 PM" src="https://github.com/user-attachments/assets/a3f9a225-a706-42a5-b963-6b2325dd821c">    

- 위와 같은 구조는 맞는데 head 노드의 prev와 end node의 next 는 NULL로 정함
- fd 값을 바탕으로 연결 리스트의 노드를 검색하여 없으면 추가하고 있으면 그 노드를 처리 대상 노드로 지정함
- 이후 처리는 mandatory와 비슷함

### void check_fd(int fd, t_list **head, t_list *head_temp, t_list **node)

- fd를 검색하고 있으면 그 노드를 처리 대상 노드로 지정, 없다면 새로운 노드를 만들고 처리 대상 노드로 지정하는 함수
- 이 함수를 만들 때는 무언가 사소한 실수로 temp 변수로 순회하는 것을 실패해서 head 변수 자체로 순회했는데 다음번 과제에서 연결 리스트를 사용하게 된다면 임시 변수를 통해서 순회하는 것이 더 좋아보임
- head를 순회해서 해당하는 fd를 발견하면 그 노드의 주소를 node 변수에 저장 (처리 대상 노드 지정)
- 그리고 순회한 head 변수는 다시 head의 원래 주소로 돌려놔야 하므로 복원

### t_list *ft_lstnew(int fd, t_list *prev)

- fd와 이전 노드의 주소를 가지고 새로운 노드를 만드는 함수
- 이 노드의 구조체 멤버 변수는 fd, str, prev, next로 구성되어 있음
- 어차피 새로운 노드를 만든다는 것은 마지막 노드이므로 next는 항상 0임
- str에  strdup를 통해서 빈 문자열을 생성 후 주소값을 할당함

### void clear_node(t_list **head, t_list **node, t_list *next_head)

- 노드를 삭제하는 함수
- head 노드 삭제 시 : head 노드의 다음 노드가 있다면 그 노드를 head 노드로 바꿔주고 그 노드의 prev는 0으로 바꿔줌
- 중간 노드 삭제 시 : 중간 노드 기준 다음 노드의 prev는 이전 노드의 주소로 바꿔주고 이전 노드의 next는 다음 노드의 주소로 바꿔줌
- 마지막 노드 삭제 시 : 이전 노드의 next를 0으로 바꿔줌
- 위 작업이 끝나면 해당 노드의 str을 free한 다음 노드를 free, 그리고 노드 포인터 변수의 값을 0으로 변경
