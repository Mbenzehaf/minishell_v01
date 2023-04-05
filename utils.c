/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mben-zeh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/29 00:56:26 by mben-zeh          #+#    #+#             */
/*   Updated: 2023/03/29 00:56:28 by mben-zeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

/*
int ft_lstsize(t_list *list,t_TokenType token,int cas)
{
    int i;

    i = 0;
	if(cas == 0)
	{
	while (list)
        i++;
        list  = list->next;
	}else if (cas==1)
	{
 	while (list && list->token == token)
    	{
        i++;
        list  = list->next;
    }
	}else 
	{
		while (list )
    	{
		if(list->token==token)
        	i++;
        list  = list->next;
		}
	}
   
    return(i);
}

void ft_strjoincmd(t_data **data,t_list* list)
{
    while (list)
    {
        list = list->next;
    }
}

int ft_counttoken(t_list *list,int token)
{
    int i;

    i = 0;
    while (list)
    {
        if(list->token == token)
            i++;
        list  = list->next;
    }
    return(i);
}
char **ft_create_arg(t_list *list)
{
    t_list *temp;
    int i;
    int size;
	char **arg;

	size = ft_counttoken(list,TOKEN_WORD);
	arg = malloc(sizeof(char *));
    i = 0;
	return NULL;
}*/

void ft_full_data(t_list *list,t_data **data)
{
	int fdin;
	int fdout;
	char *str;
	int *fd;
	char *h_doc;

	fdin = -2;
	fdout = -2;
	str = NULL;
	h_doc = NULL;
	if(!list)
		return ;
	while (list)
	{
		if (list->token == TOKEN_WORD)
		{
			str = ft_strjoin(str,(char[]){3,0});
			str = ft_strjoin(str,list->content);
		}
		else if(list->token == FILE_IN)
		{
			fdin = open(list->content,O_RDONLY);
		}else if (list->token == FILE_OUT)
		{
			fdout = open(list->content,O_CREAT|O_WRONLY|O_TRUNC,0644);
		}else if (list->token == FILE_APP)
		{
			fdout = open(list->content,O_CREAT|O_WRONLY|O_APPEND,0644);
		}else if (list->token == TOKEN_HEREDOC)
		{
			fd = malloc(sizeof(int)*2);
				if (pipe(fd))
					(perror("pipe"), exit(1));
			while (list->next)
			{
				h_doc = readline(">");
				if(h_doc && !ft_strcmp(h_doc,list->next->content))
					{
						free(h_doc);
						break;
					}
				h_doc = ft_strjoin(h_doc,"\n");
				ft_putstr(h_doc, fd[1]);
				free(h_doc);
			}
			fdin=fd[0];
			close(fd[1]);
		}
		if(list->token == TOKEN_PIPE || !list->next)
		{
			ft_dtadd_back(data,ft_dtnew(ft_split(str,3),fdin,fdout));
		}
		list = list->next;
	}
}

void ft_full_datas(t_list *list,t_data **data)
{
	int fdin;
	int fdout;
	char *str;
	int *fd;

	fdin = -2;
	fdout = -2;
	str = NULL;
	if(!list)
		return ;
	while (list)
	{
		fd = NULL;
		if(list->token == TOKEN_WORD)
		{
			str = ft_strjoin(str,(char[]){3,0});
			str = ft_strjoin(str,list->content);
		}
		else if(list->token != TOKEN_WORD)
		{
			if(list->token == FILE_IN)
				{
					if(fdin != -2)
						close(fdin);
					fdin = open(list->content,O_RDONLY);
					if(fdin == -1)
						perror(list->content);
				}
			else if(list->token == FILE_OUT)
			{
				if(fdout != -2)
					close(fdout);
				fdout = open(list->content,O_CREAT|O_WRONLY|O_TRUNC,0644);
				if(fdout == -1)
					perror(list->content);
			}
			else if(list->token == FILE_APP)
			{
				if(fdout != -2)
					close(fdout);
				fdout = open(list->content,O_CREAT|O_WRONLY|O_APPEND,0644);
				if(fdout == -1)
					perror(list->content);
			}
		}
		if(list->token == TOKEN_PIPE || !list->next)
			{
				if(list->token == TOKEN_PIPE && fdout == -2)
				{

					fd = malloc(sizeof(int)*2);
					if (pipe(fd))
						(perror("pipe"), exit(1));
					fdout = fd[1];
				}
				ft_dtadd_back(data,ft_dtnew(ft_split(str,3),fdin,fdout));
				if(fd)
				{
					fdin = fd[0];
				}else
				{
					fdin = -2;
				}
				fdout = -2;
				// if(list->token == TOKEN_PIPE && )
				// {
				// 	fd=malloc(sizeof(int)*2);
				// 	pipe(fd);
				// 	fdin = fd[0];
				// 	fdout= fd[1];
				// }
				// else{
				// 	fdin = 0;
				// 	fdout = 1;
				// 	}
				free(str);
				str = NULL;
			}
		list = list->next;
	}
}

void	ft_putstr_fd(char *s, int fd)
{
	if (!s)
		return ;
	write(fd, s, ft_strlen(s));
}

void	ft_msg_cmd_error(char *cmd)
{
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": Command not found\n", 2);
	
}

char	*ft_envchr(char **envp, char *str)
{
	int	i;

	i = 0;
	if (!envp || !str)
		return (NULL);
	while (envp[i])
	{
		if (!ft_strcmp(envp[i], str))
			return (envp[i]);
		i++;
	}
	return (NULL);
}

/*int	ft_path_envp(t_cmd *cmd_st, char **envp)
{
	char	*env;
	int		i;

	i = 0;
	env = ft_envchr(envp, "PATH=");
	if (!env)
		return (0);
	env = ft_substr(&env[5], 0, ft_strlen(&(env[5])));
	cmd_st->path = ft_split(env, ':');
	while (cmd_st->path[i])
	{
		cmd_st->path[i] = ft_strjoin(cmd_st->path[i], "/");
		i++;
	}
	free(env);
	return (1);
}*/

char *ft_getenv(t_env *env,const char *find)
{
	while (env)
	{
		if(!ft_strcmp(env->var,find))
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

char **ft_path_envp(t_env *envp)
{
	char	*env;
	char **path;
	int		i;

	i = 0;
	path = NULL;
	env = ft_getenv(envp,"PATH");
	if (!env)
		return (0);
	env = ft_substr(env, 0, ft_strlen(env));
	path = ft_split(env, ':');
	while (path[i])
	{
		path[i] = ft_strjoin(path[i], "/");
		i++;
	}
	free(env);
	return (path);
}

void ft_exec_cmd(t_data *data,t_env *env)
{
	int i;
	char *path;

	i = 0;
	if(!data->arg)
		exit(0);
	if(execve(data->arg[0], data->arg, NULL) < 0)
	{
		data->path=ft_path_envp(env);
		if (!data->path)
			{
				ft_msg_cmd_error(data->arg[0]);
				exit(127);
			}
		while (data->path[i])
			{
				path = ft_strjoin(ft_strdup(data->path[i++]), data->arg[0]);
				execve(path, data->arg, NULL);
				free(path);
			}
			ft_msg_cmd_error(data->arg[0]);
			exit(127);
	}
}

void ft_full_env(t_env **data,char **envp)
{
	int i;
	int j;
	char **env;

	i = 0;
	j = 0;
	if(!envp)
		return ;
	while (envp[i])
	{
		j=0;
		while (envp[i][j])
		{
			if(envp[i][j]=='=')
				{
					envp[i][j] = 3;
					break;
				}
			j++;
		}
		env = ft_split(envp[i],3);
		ft_envadd_back(data,ft_envnew(env[0],env[1]));
		free(env);
		i++;
	}
}

/*
void	ft_exec_cmd(t_cmd *cmd_st, char *cmd,char **env)
{
	int		i;
	char	**arg;
	char	*path;

	i = 0;
	arg = ft_split(cmd, ' ');
	if (arg)
	{
		if (execve(arg[0], arg, cmd_st->env) < 0)
		{
			if (!ft_path_envp(cmd_st, cmd_st->env))
			{
				ft_msg_cmd_error(cmd);
				exit(127);
			}
			while (cmd_st->path[i])
			{
				path = ft_strjoin(ft_strdup(cmd_st->path[i++]), arg[0]);
				execve(path, arg, cmd_st->env);
				free(path);
			}
		}
	}
	ft_msg_cmd_error(arg[0]);
	exit(127);
}
*/
