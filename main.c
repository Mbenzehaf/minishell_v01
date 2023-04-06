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
void ft_exection(t_data *data,t_env **env)
{
    static int i;
    int status;
    while (data)
    {
        data->pid = fork();
        if(data->pid == -1)
            {
                perror("fork");
                exit(EXIT_FAILURE);
            }
        if(data->pid==0)
        {
           // printf("%s\n",data->arg[0]);
          if(data->fdin > 2 && i==1)
                {
                    if(data->prev->fdout> 2)
                     close(data->fdout);
                    dup2(data->fdin,STDIN_FILENO);
                }
            if(data->fdout > 2 && i==0)
                {
                    if(data->fdin > 2)
                        close(data->prev->fdin);
                    dup2(data->fdout,STDOUT_FILENO);
                }
            i++;
            if(data->arg)
               ft_exec_cmd(data, *env);
            exit(0);
        }else{
            waitpid(data->pid ,&status,0);
        }
        data = data->next;
    }
    //exit(WIFEXITED(status));
}
int main(int ac,char *av[],char **envp)
{
    char *str;
    int i;
    t_list *list;
    t_data *data;
    t_env *env;
    
    (void)envp;
    (void)av;
    (void)ac;
    env = NULL;
    ft_full_env(&env,envp);

    while(1)
    {
        i=0;
        list = NULL;
        data = NULL;
        str = readline("Minish$>");
        ft_spl(str,&list);
        ft_full_data(list,&data);
       ft_exection(data,&env);
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
        add_history(str);
        ft_freelist(list);
       ft_freedata(data);
       // system("leaks minishell");
        if(str)
            free(str);
       
   }
}