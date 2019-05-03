# Treść:
# Pobrać ze standardowego strumienia wejściowego procesu ciąg znaków
# i zamienić wszystkie małe litery na wielkie, wszystkie wielkie na
# małe, zmienić wielkość co drugiej litery itp.

SYSCALL32 = 0x80 # wywołanie funkcji standardowej
EXIT = 1 # nr funkcji restartu (=1) – zwrot sterowania do s.o.
EXIT_SUCCESS = 0
STDIN = 0 # nr wejścia standardowego (klawiatura) do %ebx
READ = 3 # nr funkcji odczytu wejścia (=3)
STDOUT = 1 # nr wyjścia standardowego (ekran tekstowy) do %ebx
WRITE = 4 # nr funkcji wyjścia (=4)
ERR_CODE = 0
BUF_SIZE = 10 # rozmiar bufora (1 linia tekstu)

.data
    buf: .byte 0 # bufor do przechowywania wprowadzonego tekstu
    letterCount: .int 0 # licznik kolejnych liter

    askToEnter: .ascii "Enter text: \n"
    lenAskToEnter = . - askToEnter
    youveEntered: .ascii "You've entered: \n"
    lenYouveEntered = . - youveEntered
    modResultMsg: .ascii "Modified text: \n"
    lenModResultMsg = . - modResultMsg

.text
    .global _start

_start:

    # wypisz na ekranie wiadomość do użytkownika
    mov $WRITE, %eax
    mov $STDOUT, %ebx
    mov $askToEnter, %ecx
    mov $lenAskToEnter, %edx

    int $SYSCALL32

    # wczytanie dziesięciu bajtów
    mov $READ, %eax
    mov $STDIN, %ebx
    mov $buf, %ecx
    mov $BUF_SIZE, %edx

    int $SYSCALL32

    # wypisz na ekranie wiadomość do użytkownika
    mov $WRITE, %eax
    mov $STDOUT, %ebx
    mov $youveEntered, %ecx
    mov $lenYouveEntered, %edx

    int $SYSCALL32

    # wyświetlenie wczytanego tekstu
    mov $WRITE, %eax
    mov $STDOUT, %ebx
    mov $buf, %ecx
    mov $BUF_SIZE, %edx

    int $SYSCALL32

    # wypisz na ekranie wiadomość do użytkownika
    mov $WRITE, %eax
    mov $STDOUT, %ebx
    mov $modResultMsg, %ecx
    mov $lenModResultMsg, %edx

    int $SYSCALL32

    jmp _checkParityModifyThanIncrement

# modyfikuj co drugą literę
_checkParityModifyThanIncrement:
    movl $0, %edx
    mov $letterCount, %eax
    mov (%eax), %ebx
    mov $2, %ebx
    divl %ebx # modulo w EDX

    movl $letterCount, %eax
    inc %eax
    movl %eax, letterCount

    mov $0, %eax
    cmp %edx, %eax
    jz _modifyToUppercase

    movl letterCount, %eax
    cmpl $10, %eax 
    jg _exit

    jmp _checkParityModifyThanIncrement

_modifyToUppercase:
    mov letterCount, %edx
    mov buf(,%edx,4), %eax
    mov $32, %ebx
    add %eax, %ebx
    mov %eax, buf(,%edx,4)
    jmp _checkParityModifyThanIncrement

_exit:
    # wyświetlenie wczytanego tekstu
    mov $WRITE, %eax
    mov $STDOUT, %ebx
    mov $buf, %ecx
    mov $BUF_SIZE, %edx

    int $SYSCALL32

    # zakończenie wykonywania programu
    mov $EXIT, %eax 
    mov $EXIT_SUCCESS, %ebx

int $SYSCALL32
