_DATA SEGMENT
	hello_msg db "Hello world", 0
	info_msg  db "Info", 0
_DATA ENDS
 
; -----------------------------------------------------------------------------------------------------------	
; Text or code segment
; -----------------------------------------------------------------------------------------------------------	
_TEXT	SEGMENT
 
; Needed windows functions to show a MessageBox
EXTERN MessageBoxA: PROC
EXTERN GetForegroundWindow: PROC
 
; The PUBLIC modifier will make your function visible and callable outside
PUBLIC hello_world_asm
 
hello_world_asm PROC
 
	push rbp ; save frame pointer
	mov rbp, rsp ; fix stack pointer
	;sub rsp, 8 * (4 + 2) ; allocate shadow register area + 2 QWORDs for stack alignment
	sub rsp, 32
	; Get a window handle
	call GetForegroundWindow
	add rsp, 32
	mov rcx, rax ; rax contains window handle
 
	; WINUSERAPI int WINAPI MessageBoxA(
	;  RCX =>  _In_opt_ HWND hWnd,
	;  RDX =>  _In_opt_ LPCSTR lpText,
	;  R8  =>  _In_opt_ LPCSTR lpCaption,
	;  R9  =>  _In_ UINT uType);
 
	mov rdx, offset hello_msg
	mov r8, offset info_msg
	mov r9, 0 ; MB_OK
 
	sub rsp, 32
	;and rsp, not 8 ; align stack to 16 bytes prior to API call
	;sub rsp, 8 ; mess up the alignment
	call MessageBoxA
	add rsp, 32
	; epilog. restore stack pointer
	mov rsp, rbp
	pop rbp
 
	ret	
hello_world_asm ENDP
 
 
_TEXT	ENDS
 
END

