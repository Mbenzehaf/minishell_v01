/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mben-zeh <mben-zeh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 00:51:30 by mben-zeh          #+#    #+#             */
/*   Updated: 2023/04/22 09:24:20 by mben-zeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int ft_check_n(char *str)
{
    int i;

    i = 0;
    if(str[i++]!='-')
        return (1);
    
    while (str[i])
    {
        if(str[i]!='n')
            return (1);
        i++;
    }
    return(0);
}

void ft_echo(char **str)
{
    int n;
    int i;

    n = 0;
   
    while (str[n] && !ft_check_n(str[n]))
        n++;
    i = n;
    while (str[i])
    {
        ft_putstr(str[i],1);
        if(str[i + 1])
            ft_putstr(" ",1);
        i++;
    }
    if(!n)
        ft_putstr("\n",1);
}

void ft_cd(char *str)
{
    
}
int is_var(char *str,int *equal,int *add)
{
    int i;
    *equal = 0;
    i = 0;
    if(!ft_isalpha(str[i]) && str[i]!='_')
        return (1);
    while (str[i])
    {
        if(!ft_isalpha(str[i]) && !ft_isdigit(str[i]) && str[i]!= '=' && str[i]!= '_' && !*equal && str[i]!= '+' )
            return(1);
        if(!*equal && str[i]=='+' && str[i + 1]== '=')
            {
                *add = 1;
                str[i] = 3;
            }
        if(!*equal && str[i]=='=')
            {
                *equal = 1;
                str[i] = 3;
            }
        i++;
    }
    return (0);
}
int ft_search_inset(t_env *env,char **str,int equal,int add)
{
while(env)
{
    if(!ft_strcmp(env->var,str[0]))
    {
        if(str[1])
            {
                if(env->value && !add)
                    free(env->value);
                if(add)
                env->value = ft_strjoin(env->value,str[1]);
                else
                env->value = str[1];
            }
       else if(equal)
           {
             if(env->value && !add)
                    free(env->value);
            if(!add)
                env->value =ft_strdup("");
            
           }
        return(0);
    }
    env = env->next;
}
return(1);
}

char *ft_search_env(t_env *env,char *str)
{
    while(env)
    {
        if(!ft_strcmp(env->var,str))
            return(env->value);
    env = env->next;
    }
    return(NULL);
}

void ft_export(t_env **env, char **str)
{
    int i;
    char **arg;
    int equal;
    int add;

    equal = 0;
    add = 0;
    i = 0;
    while (str[i])
    {
        if(!is_var(str[i],&equal,&add))
        {
            arg = ft_split(str[i],3);
            if(ft_search_inset(*env,arg,equal,add))
            {
            ft_envadd_back(env,ft_envnew(arg[0],arg[1]));
            }else
                free(arg[0]);
            free(arg);
        }else
        {
            ft_putstr("export: `", 2);
            ft_putstr(str[i], 2);
            ft_putstr("': not a valid identifier\n", 2);
        }
        i++;
    }
    
    
}

void ft_unset(t_env *env, char *str)
{
    t_env *node;

    node = env;
    while (env)
    {
        if(!ft_strcmp(env->var,str))
        {
            node->next = env->next;
            env->next=NULL;
            free(env->value);
           free(env->var);
          free(env);
            break ;
        }
        node = env;
        env = env->next;
    }
    
}

void ft_env(t_env *env,int is_export)
{
    while (env)
    {
         if(is_export || env->value)
        {
            if(is_export)
            {
                ft_putstr("declare -x ",1);
                ft_putstr(env->var,1);
                if(env->value)
                {
                ft_putstr("=",1);
                ft_putstr("\"",1);
                ft_putstr(env->value,1);
                ft_putstr("\"",1);
                }

            }
            else if(env->value)
            {
            ft_putstr(env->var,1);
            ft_putstr("=",1);
            ft_putstr(env->value,1);
            }
             ft_putstr("\n",1);
        }
        env = env->next;
    }   
}

//◦ echo with option -n
//◦ cd with only a relative or absolute path ◦ pwd with no options
//◦ export with no options
//◦ unset with no options
//◦ env with no options or arguments
//◦ exit with no options
