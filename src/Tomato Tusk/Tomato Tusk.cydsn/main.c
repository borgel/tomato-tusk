#include <project.h>

#include <stdbool.h>
int main()
{
    CyGlobalIntEnable;
    
    // init uart
    UART_Start();
    UART_PutString("Tomato Tusk [__DATE__] Starting...\n");

    for(;;)
    {
    }
}
