Stack Smashing Assignment
=========================

The `ctarget` binary reads string from STDIN using getbuf funtion.
`unsigned getbuf(void){  
    char buf[BUFFER_SIZE];  
    Gets(buf);  
    return 1;  
}`  
The `Gets` `gets` implementation does not check the buffer size.
Basic analyze of `getbuf` deassembled code:
`000000000040174a <getbuf>:
  40174a:	48 83 ec 18          	sub    $0x18,%rsp
  40174e:	48 89 e7             	mov    %rsp,%rdi
  401751:	e8 44 02 00 00       	callq  40199a <Gets>
  401756:	b8 01 00 00 00       	mov    $0x1,%eax
  40175b:	48 83 c4 18          	add    $0x18,%rsp
  40175f:	c3                   	retq`
tells you that buffer takes up 0x18=24 bytes on the stack ie. 24 characters.
Thus, when more than 24 characters are typed in by user, the 25th character overrides adjacent stack element, which is caller return address used by retq instruction. 
## Task 1

## Task 2

## Task 3