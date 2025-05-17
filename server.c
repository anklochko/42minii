/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anastasiya <anastasiya@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 16:13:07 by anastasiya        #+#    #+#             */
/*   Updated: 2025/05/14 17:50:08 by anastasiya       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Loop the server can receive signals at any time
// Receive signals
// Decrypt signals
// For each signal received (SIGUSR1 & SIGUSR2) it should take a certain action

//Server — это тот, кто принимает сигнал и из него восстанавливает сообщение.
//Он получает сигналы, считает биты, складывает их, и после 8 битов — выводит один символ.

#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void signal_handler(int signal, siginfo_t *info, void *context)
{
    static int bit = 0;
    static unsigned char current_char = 0; //это разобранный чар
    (void)context;
    current_char = current_char << 1; // было 01000000 стало 10000000 

    if(signal ==  SIGUSR2) // 1 bit not 0
        current_char = current_char | 1;
    bit++;
    if(bit == 8) // change signals we have (8 bits) actually to char
    {
        if (current_char == '\0')
            write(1, "\n", 1); 
        else 
            write(1, &current_char, 1);
        current_char = 0;
        bit = 0;
    }
    kill(info->si_pid, SIGUSR1);
    usleep(40); 
}

int main()
{
    pid_t pid;
    pid = getpid();

    struct sigaction sa; 

    sa.sa_sigaction = signal_handler;
    sa.sa_flags = SA_SIGINFO;
    sigemptyset(&sa.sa_mask); 
    // sigaddset(&sa.sa_mask, SIGUSR1); //не перебивай сигнал бит 1 или бит 0
	// sigaddset(&sa.sa_mask, SIGUSR2);
    
    printf("Hey, PID number is %d\n", pid); 
    printf("Waiting for signals...\n");

    sigaction(SIGUSR1, &sa, NULL);
    sigaction(SIGUSR2, &sa, NULL);
    while (1) {
        pause(); 
    }
    return 0;
}