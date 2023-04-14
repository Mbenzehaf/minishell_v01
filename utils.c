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
int exit_status = 0;
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
char *ft_split_join(char *str)
{
	int i;
	char **ptr;
	char *tab;
	i = 0;

	tab = NULL;
	ptr = ft_split(str,3);

	while (ptr[i])
	{
		tab = ft_strjoin(tab,ptr[i]);
		i++;
	}
	i = 0;
	while (ptr && ptr[i])
			i++;
	while(i)
			free(ptr[--i]);
		free(ptr);
	return (tab);
}

char *get_expand(t_env *env,char *str)
{
	//printf("---->%s\n",str);
	if(!str||!*str)
		return("$");
	if(!ft_strcmp(str,"?"))
		{
			free(str);
			return (ft_itoa(exit_status));
		}
	while (env && str)
	{
		if(!ft_strcmp(str,env->var))
		{
			free(str);
			return (env->value);
		}
		env = env->next;
	}
	free(str);
	return (NULL);
}

char *ft_expand_quote(char *str,t_env *env)
{
	int i;
	int quote;
	int dquote;
	char *string;
	char *ex_str;
	int j;
	
	quote = 0;
	dquote = 0;
	i = 0;
	string = NULL;
	while(str[i])
	{
		if( str[i]=='\"' && !(quote % 2))
				dquote++;
		else if( str[i]=='\'' && !(dquote % 2))
				quote++;
		if(str[i]=='$' && !(quote % 2) && str[i + 1 ]!='\'' &&((dquote % 2) || str[i + 1]!= '\"'))
		{ 
			j = 0;
			while(str[i+(++j)])
			{
				 if ((str[i + 1]=='?' && str[i]=='$'))
			{
					j++;
					break;
			}
			else if(!(ft_isalpha(str[i+j]) || ft_isdigit(str[i + j])||str[i + j]=='_'))
						break ;  
			}
			ex_str = get_expand(env,ft_substr(str,i + 1 ,j-1));
			string = ft_strjoin(string , ex_str);
			if (str[i + 1]=='?')
				free(ex_str);

			//leaks in $?
			i = i + j - 1;
		}else if((str[i]=='\"' && (quote % 2))||(str[i]=='\'' && (dquote % 2))||(str[i]!='\"' && str[i]!='\'' && (str[i]!='$'||(quote % 2))))
			string =ft_strjoin(string,(char[]){str[i],0});
		i++;
	}
	free(str);
	return(string);
}

void ft_exection(t_data *data,t_env **env,char **envp)
{
    int status;
    t_data *temp;
    temp = data;

	if(data &&data->arg && *data->arg && !ft_strcmp(data->arg[0],"exit"))
		(ft_putstr("exit\n",1),exit(0));
    while (data)
    {
		if(data->next)
		{
			data->fd=malloc(2*sizeof(int));
				pipe(data->fd);
		}
        data->pid = fork();
        if(data->pid == -1)
                (perror("fork"), exit(EXIT_FAILURE));
        if(data->pid==0)
        {
            if(data->fdin==-1||data->fdout==-1)
                exit(1);
          if(data->fdin > 2 )
                {
                   //if(data->prev && data->prev->fdout > 2)
                       // close(data->prev->fdout);
                    dup2(data->fdin,STDIN_FILENO);
                }else if(data->prev)
				{
					 if(data->prev)
                        close(data->prev->fd[1]);
                    dup2(data->prev->fd[0],STDIN_FILENO);	
				}
            if(data->fdout > 2)
                {
                    dup2(data->fdout,STDOUT_FILENO);
                }else if(data->fd)
				{
                    close(data->fd[0]);
                    dup2(data->fd[1],STDOUT_FILENO);
				}
            ft_exec_cmd(data, *env,envp);
            exit(0);
        }else{
			if(data->prev)
				{
				close(data->prev->fd[0]);
				close(data->prev->fd[1]);
				}
        }
        data = data->next;
    }
    while(temp)
    {
		if(temp->fdin > 2)
                close(temp->fdin);
       if(temp->fdout > 2)
                close(temp->fdout);
         waitpid(temp->pid,&status,0);
        temp = temp->next;
	}
    exit_status = WEXITSTATUS(status);

}
/*
char *ft_expand_and_quote(char *str)
{
	int i;
	int quote;
	int dquote;
	char **ptr;
	char *string;
	int is_edit;

	quote = 0;
	dquote = 0;
	is_edit = 0;
	i = 0;
	while (str[i])
	{
		if( str[i]=='\"' && !(quote % 2))
			{
				str[i] = 3;
				dquote++;
				is_edit = 1;
			}
		else if( str[i]=='\'' && !(dquote % 2))
			{
				str[i] = 3;
				quote++;
				is_edit = 1;
			}
		if(str[i]=='$')
		{
			
			is_edit = 1;
		}
		i++;
	}
	if(is_edit)
		return (ft_split_join(str));
	else
		return (str);
}
*/

void ft_full_data(t_list *list,t_list *temp,t_data **data)
{
	int fdin;
	char *str;
	int fd[2];
	char *h_doc;
	int count_fdout;

	fdin = -2;
	count_fdout = 0;
	str = NULL;
	h_doc = NULL;
	while (list)
	{
		if (list->token == TOKEN_WORD)
			str = ft_strjoin(ft_strjoin(str,(char[]){3,0}),list->content);
		else if(list->token == FILE_IN)
		{
			if(fdin > 2)
				close(fdin);
			fdin = open(list->content,O_RDONLY);
			if(fdin==-1)
				perror(list->content);
		}else if (list->token == FILE_OUT || list->token == FILE_APP)
			count_fdout++;
		else if (list->token == TOKEN_HEREDOC)
		{
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
				(ft_putstr(h_doc, fd[1]), free(h_doc));
			}
			fdin=fd[0];
			close(fd[1]);
		}
		if(list->token == TOKEN_PIPE || !list->next)
		{
			ft_dtadd_back(data,ft_dtnew(ft_split(str,3),fdin,count_fdout));
			free(str);
			str = NULL;
			fdin = -2;
		}
		list = list->next;
	}
	
	ft_fdout(temp,*data);
}

void ft_full_datassss(t_list *list,t_data **data)
{
	int fdin;
	int fdout;
	char *str;
	int fd[2];
	char *h_doc;
	int count_fdout;

	fdin = -2;
	fdout = -2;
	count_fdout = 0;
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
			//str = ft_strjoin(ft_strjoin(str,(char[]){3,0}),list->content);
		}
		else if(list->token == FILE_IN)
		{
			if(fdin > 2)
				close(fdin);
			fdin = open(list->content,O_RDONLY);
			if(fdin==-1)
				perror(list->content);
		}else if (list->token == FILE_OUT)
		{
			count_fdout++;
			if(fdout > 2)
				close(fdout);
			fdout = open(list->content,O_CREAT|O_WRONLY|O_TRUNC,0644);
			if(fdout==-1)
				perror(list->content);
		}else if (list->token == FILE_APP)
		{
			count_fdout++;
			if(fdout > -2)
				close(fdout); 
			fdout = open(list->content,O_CREAT|O_WRONLY|O_APPEND,0644);
			if(fdout==-1)
				perror(list->content);
		}else if (list->token == TOKEN_HEREDOC)
		{
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
					//printf("---->%s\n",h_doc);
				h_doc = ft_strjoin(h_doc,"\n");
				ft_putstr(h_doc, fd[1]);
				free(h_doc);
			}
			fdin=fd[0];
			close(fd[1]);
		}
		if(list->token == TOKEN_PIPE || !list->next)
		{
			/*if(list->token == TOKEN_PIPE)
				{
					if(fdout <0)
					{
					if (pipe(fd))
						(perror("pipe"), exit(1));
					fdout = fd[1];
					is_pipe = 1;
					}
				}*/
			//if(str || fdout>2)
			ft_dtadd_back(data,ft_dtnew(ft_split(str,3),fdin,count_fdout));
			free(str);
			str = NULL;
			/*if(is_pipe)
			{
				fdin = fd[0];
				is_pipe = 0;
			}else
				fdin = -2;*/
			fdout = -2;
			fdin = -2;
		}
		list = list->next;
	}
}
void ft_fdout(t_list *list,t_data *data)
{
	int fdout;

	fdout = -2;
	if(!list)
		return ;
	while (list)
	{
		if (list->token == FILE_OUT)
		{
			if(fdout > 2)
				close(fdout);
			fdout = open(list->content,O_CREAT|O_WRONLY|O_TRUNC,0644);
			if(fdout==-1)
				perror(list->content);
		}else if (list->token == FILE_APP)
		{
			if(fdout > -2)
				close(fdout); 
			fdout = open(list->content,O_CREAT|O_WRONLY|O_APPEND,0644);
			if(fdout==-1)
				perror(list->content);
		}
		if(data && !data->n_fdout)
			{
				data = data->next;
				fdout = -2;
			}
		else if(fdout > 2)
		{
			data->fdout = fdout;
			data->n_fdout--;
		}
		list=list->next;
	}
}
void ft_full_datass(t_list *list,t_data **data)
{
	int fdin;
	int fdout;
	char *str;
	int fd[2];
	char *h_doc;
	int is_pipe;

	fdin = -2;
	fdout = -2;
	is_pipe = 0;
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
			//str = ft_strjoin(ft_strjoin(str,(char[]){3,0}),list->content);
		}
		else if(list->token == FILE_IN)
		{
			if(fdin > 2)
				close(fdin);
			fdin = open(list->content,O_RDONLY);
			if(fdin==-1)
				perror(list->content);
		}else if (list->token == FILE_OUT)
		{
			if(fdout > 2)
				close(fdout);
			fdout = open(list->content,O_CREAT|O_WRONLY|O_TRUNC,0644);
			if(fdout==-1)
				perror(list->content);
		}else if (list->token == FILE_APP)
		{
			if(fdout > -2)
				close(fdout); 
			fdout = open(list->content,O_CREAT|O_WRONLY|O_APPEND,0644);
			if(fdout==-1)
				perror(list->content);
		}else if (list->token == TOKEN_HEREDOC)
		{
			//fd = malloc(sizeof(int)*2);
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
			if(list->token == TOKEN_PIPE)
				{
					if(fdout <0)
					{
					if (pipe(fd))
						(perror("pipe"), exit(1));
					fdout = fd[1];
					is_pipe = 1;
					}
				}
			ft_dtadd_back(data,ft_dtnew(ft_split(str,3),fdin,fdout));
			free(str);
			str = NULL;
			if(is_pipe)
			{
				fdin = fd[0];
				is_pipe = 0;
			}else
				fdin = -2;
			fdout = -2;
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

void ft_exec_cmd(t_data *data,t_env *env,char **envp)
{
	int i;
	char *path;

	i = 0;
	if(!data->arg||!*data->arg)
		{
			ft_msg_cmd_error(data->arg[0]);
			exit(127);
		}
		if(!ft_strcmp(data->arg[0],"exit"))
			return ; 
	if(execve(data->arg[0], data->arg, envp) < 0)
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
				execve(path, data->arg, envp);
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
