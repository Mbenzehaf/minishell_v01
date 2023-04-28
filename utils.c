/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mben-zeh <mben-zeh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/29 00:56:26 by mben-zeh          #+#    #+#             */
/*   Updated: 2023/04/22 09:29:26 by mben-zeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *get_expand(t_env *env,char *str,int dquote)
{
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
	if(dquote % 2 == 0)
		return ((char[]){127,0});
	else
		return (NULL);
}

char *ft_expand_quote(char *str,t_env *env,int isheredoc,int is_lim)
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
		if(str[i]=='$' && !(quote *isheredoc % 2) && str[i + 1 ]!='\'' &&((dquote*isheredoc % 2) || str[i + 1]!= '\"') && !is_lim)
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
			ex_str = get_expand(env,ft_substr(str,i + 1 ,j-1),dquote * isheredoc);
			string = ft_strjoin(string , ex_str);
			if (str[i + 1]=='?')
				free(ex_str);
			//leaks in $?
			i = i + j - 1;
		}else if(((str[i]=='\"' && (quote  % 2))||(str[i]=='\'' && (dquote % 2))||(str[i]!='\"' && str[i]!='\'' && (str[i]!='$'||(quote % 2)||is_lim)))||!isheredoc)
			string =ft_strjoin(string,(char[]){str[i],0});
		i++;
	}
	free(str);
	return(string);
}

int ft_builtins(t_data *data,t_env *env,int x)
{
	char *str;
	str = NULL;
	if(!ft_strcmp("echo",data->arg[0]))
	{
		ft_echo(&data->arg[1]);
		return (1);
	}else if (!ft_strcmp("pwd",data->arg[0]))
	{
		ft_putstr(getcwd(NULL,0),1);
		ft_putstr("\n",1);
		return(1);
	}else if (!ft_strcmp("env",data->arg[0]) && !data->arg[1])
	{	
		ft_env(env,0);
		return(1);
	}else if (!ft_strcmp("export",data->arg[0]))
	{
		if(!data->arg[1])
			ft_env(env,1);
		else
			;
		return(1);
		
	}else if(!ft_strcmp("cd",data->arg[0]))
	{

	}
	return (0);
}

int builtins(char **str)
{
	return (0);
}
int size_env(t_env *env)
{
	int i;

	i = 0;
	while (env)
	{
		if(env->value)
			i++;
		env = env->next;
	}
	return (i);
}

char **ft_env_tab(t_env *env)
{
	char **ptr;
	int i;
	int len;

	i = 0;
	len = size_env(env);
	ptr = malloc(sizeof(char *) * (len + 1));
	while (i < len)
	{
		if(env->value)
		{
			ptr[i] = ft_strjoin(ft_strdup(env->var),"=");
			ptr[i] = ft_strjoin(ptr[i],env->value);
			i++;
		}
		env = env->next;
	}
	ptr[i] = NULL;
	return (ptr);
	
}

void ft_wait_close(t_data *data)
{
	int status;
	while(data)
    {
		if(data->fdin > 2)
                close(data->fdin);
       if(data->fdout > 2)
                close(data->fdout);
         waitpid(data->pid,&status,0);
        data = data->next;
	}
    exit_status = WEXITSTATUS(status);
}

void ft_exection(t_data *data,t_env **env)
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
		}else
			{
				if(!data->prev && ft_builtins(data,*env,1))
				{

				}
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
			
			if(!ft_builtins(data,*env,0))
            	ft_exec_cmd(data, *env);
			//system("leaks minishell");
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
void    ssig_handler(int sig)
{
    if (sig == SIGINT)
    {
       ft_putstr("\n",1);
        rl_on_new_line();
        rl_replace_line("", sig);
        rl_redisplay();
    }
}

void ft_full_heredoc(t_heredoc **heredoc,t_list *list,t_env *env)
{
	int fd[2];
	char *h_doc;
	pid_t pid ;

	*heredoc = NULL;
	h_doc = NULL;
	
	while (list)
	{
		if (list->token == TOKEN_HEREDOC)
		{
			if (pipe(fd))
				(perror("pipe"), exit(EXIT_FAILURE));
			pid = fork();
			if(pid == -1)
				(perror("fork"), exit(EXIT_FAILURE));
			if(pid==0){
			close(fd[0]);
			while (list->next)
			{
				h_doc = readline("> ");
				if(!h_doc || !ft_strcmp(h_doc,list->next->content))
					{
						free(h_doc);
						break;
					}
				h_doc = ft_expand_quote(h_doc,env,0,0);
				h_doc = ft_strjoin(h_doc,"\n");
				(ft_putstr(h_doc, fd[1]), free(h_doc));
			}
			exit(0);
			}
			else
			{
				wait(0);
				//kill(pid,0);
				ft_hdocadd_back(heredoc,ft_hdocnew(fd[0]));
				close(fd[1]);
			}
			
		}
		list = list->next;
	}
}

void ft_full_heredocc(t_heredoc **heredoc,t_list *list,t_env *env)
{
	int fd[2];
	char *h_doc;

	*heredoc = NULL;
	h_doc = NULL;
	
	while (list)
	{
		if (list->token == TOKEN_HEREDOC)
		{
			if (pipe(fd))
				(perror("pipe"), exit(EXIT_FAILURE));
			while (list->next)
			{
				h_doc = readline(">");
				if(!h_doc || !ft_strcmp(h_doc,list->next->content))
					{
						free(h_doc);
						break;
					}
				h_doc = ft_expand_quote(h_doc,env,0,0);
				h_doc = ft_strjoin(h_doc,"\n");
				(ft_putstr(h_doc, fd[1]), free(h_doc));
			}
			ft_hdocadd_back(heredoc,ft_hdocnew(fd[0]));
			close(fd[1]);
		}
		list = list->next;
	}
}
t_heredoc *free_next(t_heredoc *heredoc)
{
	t_heredoc *temp;
	temp = heredoc;
	heredoc = heredoc->next;
	free(temp);
	return (heredoc);
}
void ft_full_data(t_list *list,t_data **data,t_env *env)
{
	int fdin;
	int fdout;
	t_heredoc *heredoc;
	char *str;

	fdin = -2;
	fdout = -2;
	str = NULL;
	ft_full_heredoc(&heredoc, list,env);
	while (list)
	{
		if (list->token == TOKEN_WORD && ft_strcmp(list->content,(char[]){127,0}))
			{
				if(!list->content)
					str = ft_strjoin(ft_strjoin(str,(char[]){3,0}),(char[]){127,0});
				else
					str = ft_strjoin(ft_strjoin(str,(char[]){3,0}),list->content);
			}
		if(list->token == FILE_IN && fdin != -1 && fdout !=-1)
		{
			if(fdin > 2)
				close(fdin);
			fdin = open(list->content, O_RDONLY);
			if(fdin == -1)
				perror(list->content);
		}
		else if (list->token == FILE_OUT && fdin != -1 && fdout !=-1)
		{
			if(fdout > 2)
				close(fdout);
			fdout = open(list->content,O_CREAT|O_WRONLY|O_TRUNC,0644);
			if(fdout==-1)
				perror(list->content);
		}else if (list->token == FILE_APP && fdin != -1 && fdout !=-1)
		{
			if(fdout > 2)
				close(fdout); 
			fdout = open(list->content,O_CREAT|O_WRONLY|O_APPEND,0644);
			if(fdout==-1)
				perror(list->content);
		}
		else if (list->token == TOKEN_HEREDOC)
		{
			if(fdin > 2)
				close(fdin);
			if(fdin != -1)
				fdin = heredoc->fd;
			else
				close(heredoc->fd);
			heredoc = free_next(heredoc);
		}
		if(list->token == TOKEN_PIPE || !list->next)
		{
			if(fdin != -1 && fdout !=-1)
				ft_dtadd_back(data,ft_dtnew(ft_split(str,3),fdin,fdout));
			else
				ft_dtadd_back(data,ft_dtnew(NULL,0,1));
			free(str);
			str = NULL;
			fdin = -2;
			fdout = -2;
		}
		list = list->next;
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

void	ft_putstr_fd(char *s, int fd)
{
	if (!s)
		return ;
	write(fd, s, ft_strlen(s));
}

void	ft_msg_cmd_error(char *cmd)
{
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": command not found\n", 2);
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
	char **envp;

	i = 0;
	if(!data->arg||!*data->arg)
		{
			if(data->arg)
				ft_msg_cmd_error(data->arg[0]);
			exit(127);
		}
		if(data->arg && !ft_strcmp(data->arg[0],"exit"))
			return ;
		envp = ft_env_tab(env);
	if(execve(data->arg[0], data->arg,envp) < 0)
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
		envp[i][j]='=';
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
