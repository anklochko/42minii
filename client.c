/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anastasiya <anastasiya@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 17:44:57 by anastasiya        #+#    #+#             */
/*   Updated: 2025/05/14 17:51:28 by anastasiya       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Write a program (main) in which the client takes two parameters/arguments
// The PID of the server to which it wants to send the message
//              Encrypt the message (I did the encryption via bits)
//              Send the message to the server (via its PID)
//              Create a stop condition so that the server knows when it has finished receiving the message

//Client — это тот, кто передаёт сообщение (твоё сообщение — это строка).
//Он берёт каждый символ строки, разбивает его на 8 бит, 
//и по каждому биту отправляет сигнал SIGUSR1 (если бит 0) или SIGUSR2 (если бит 1) серверу.


#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <ctype.h>
#include "libft.h"

static int g_ack;

void confirm(int sig)
{
    (void)sig;
    g_ack = 1;
}

void please_send_char(pid_t pid, char *str)
{
    int i = 0;
    int bit;
    unsigned char c;
    
    while(1)
    {
        bit = 8;
        c = str[i];
        while(bit--)
        {
            g_ack = 0;
            if ((c >> bit) & 1)
                kill(pid, SIGUSR2); // 1
            else 
                kill(pid, SIGUSR1); //0
            while(g_ack != 1)
                usleep(50);
        }
        if(c == '\0')
            break ; 
        i++;    
    }
}

int main(int argc, char **argv)
{
    struct sigaction sa;
    
    if(argc != 3)    // prog name   // pid   // str
    {
        printf("Usage: %s <server_pid> <message>.\n", argv[0]);
        return 1;
    }
    pid_t pid = atoi(argv[1]); // преобразуем строку в числоpid_t pid = atoi(argv[1]); // преобразуем строку в число
    sa.sa_handler = confirm;
    sa.sa_flags = SA_RESTART;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGUSR1, &sa, NULL);
    
    please_send_char(pid, argv[2]);
    return 0;
}