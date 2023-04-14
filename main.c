/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mben-zeh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 23:08:38 by mben-zeh          #+#    #+#             */
/*   Updated: 2023/03/22 23:08:40 by mben-zeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

/*void ft_fill(t_list *list,char **str)
{
    int i;
    i=0;
    while (str[i])
    {
        i++;
    }
    
}*/
/*
void ft_minishell(t_list *list)
{
    int pipe;
    int fd[2];
    

    pipe = ft_counttoken(list,TOKEN_PIPE) + 1;

    while (pipe)
    {
        int pid =fork();
        if(pid == -1)
        {
            exit(1);
        }
        if(pid == 0)
        {

        }
        pipe--;
    }
}*/
void sigint_handler(int signal) {
    (void)signal;
    return ;
}
int main(int ac,char *av[],char **envp)
{
    char *str;
    int i;
    t_list *list;
    t_data *data;
    t_env *env;

    (void)av;
    (void)ac;
    env = NULL;
    ft_full_env(&env,envp);
    /*if (signal(SIGINT, sigint_handler) == SIG_ERR) {
        perror("signal");
        exit(1);
    }*/
    
    while(1)
    {
        i=0;
        list = NULL;
        data = NULL;
        str = readline("Minish$>");
        if(str)
        {
        ft_spl(str,&list,env);
        ft_full_data(list,list,&data);
        ft_exection(data,&env,envp);
        add_history(str);
        ft_freelist(list);
        ft_freedata(data);
        free(str);
        }
    
   }
   //rl_clear_history(); 
}
 /*while (list)
        {
        printf("(%s,%i)\n",list->content,list->token);  
            list = list->next;
        }*/
    /*while (data) 
        {
            i = 0;
            while (data->arg[i])
            {
                printf("(%s,",data->arg[i]);
                i++;
            }
             printf(")\n");
            data = data->next;
        }*/