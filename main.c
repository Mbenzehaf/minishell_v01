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
char *ft_inspace(char *str)
{
    int i;
    char *newstr;

    newstr = NULL;
    i = 0;
    while (str[i])
    {
        i++;
    }
    return newstr;
}

int main(int ac,char *av[],char **env)
{
    char *str;
    int i;
    t_list *list;
    list = NULL;

    while(1)
    {
        i=0;
        str=readline("Minishell: ");
     ft_subadd(str,&list);
    while (list)
        {
            printf("--->%s--->%i\n",list->content,list->token);
            list = list->next;
        }
        add_history(str);
        free(str);
   }


}