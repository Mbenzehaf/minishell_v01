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

void    sig_handler(int sig)
{
    if (sig == SIGINT)
    {
       ft_putstr("\n",1);
        rl_on_new_line();
        rl_replace_line("", sig);
        rl_redisplay();
    }
}
int main(int ac,char *av[],char **envp)
{
    char *str;
    t_list *list;
    t_data *data;
    t_env *env;
    int fd;

    (void)av;
    (void)ac;
    env = NULL;
    ft_full_env(&env,envp);
   if (signal(SIGINT, sig_handler) == SIG_ERR) {
        perror("signal");
        exit(1);
    }

    while(1)
    {
        list = NULL;
        data = NULL;
        
        str = readline("\x1b[1;36mMinish$>\x1b[1;0m \x1b[1;38m");
       ft_putstr("\x1b[0;0m",1);
        if(str)
        {
        ft_spl(str,&list,env);
        
        ft_full_data(list,&data,env);
        ft_exection(data,&env);
        add_history(str);
        ft_freelist(list);
        ft_freedata(data);
        free(str);
        }else
        {
            ft_putstr("exit\n",1);
            break;
        }
   }
}