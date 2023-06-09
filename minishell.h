/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mben-zeh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 23:08:59 by mben-zeh          #+#    #+#             */
/*   Updated: 2023/03/22 23:09:01 by mben-zeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef MINISHELL_H
# define MINISHELL_H
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
int						exit_status;
typedef enum TokenType
{
	TOKEN_PIPE = 0,
	TOKEN_DPIPE = 1,
	TOKEN_APPEND = 2,
	TOKEN_HEREDOC = 3,
	TOKEN_INPUT = 4,
	TOKEN_OUTPUT = 5,
	TOKEN_WORD = 6,
	FILE_IN = 7,
	FILE_OUT = 8,
	FILE_APP = 9,
	WORD_LIM = 10,
}						t_TokenType;

typedef struct s_data
{
	char				**path;
	int					fdout;
	int					fdin;
	int					*fd;
	char				**arg;
	struct s_data		*next;
	struct s_data		*prev;
	pid_t				pid;
}						t_data;

typedef struct s_list
{
	char				*content;
	t_TokenType			token;
	struct s_list		*next;
	struct s_list		*prev;
}						t_list;

typedef struct s_env
{
	char				*var;
	char				*value;
	struct s_env		*next;
}						t_env;

typedef struct s_heredoc
{
	int					fd;
	struct s_heredoc	*next;
}						t_heredoc;

void	ft_lstadd_back(t_list **lst, t_list *new ,t_env *env);
t_list					*ft_lstnew(char *content);
char					*ft_substr(char const *s, unsigned int start,
							size_t len);
void					ft_subadd(char *str, t_list **list);
int						ft_spl(char *str, t_list **list, t_env *env);
int						ft_lstsize(t_list *list, int token, int cas);
char					*ft_strjoin(char *s1, char *s2);
char					*ft_strdup(const char *s1);
//t_data				*ft_dtnew(char **arg, int in, int out);
t_data					*ft_dtnew(char **arg, int in, int out);
void					ft_dtadd_back(t_data **lst, t_data *new);
char					**ft_split(char const *s, char c);
//void				ft_full_data(t_list *list,t_list *temp, t_data **data);
void					ft_fdout(t_list *list, t_data *data);
void					ft_freelist(t_list *lst);
void					ft_freedata(t_data *lst);
char					**ft_path_envp(t_env *envp);
t_env					*ft_envnew(char *var, char *value);
void					ft_envadd_back(t_env **lst, t_env *new);
void					ft_full_env(t_env **data, char **envp);
size_t					ft_strlen(const char *str);
int						ft_strcmp(const char *s1, const char *s2);
void					ft_exec_cmd(t_data *data, t_env *env);
void					ft_putstr(char *s, int fd);
char					*ft_expand_and_quote(char *str);
char *ft_expand_quote(char *str,t_env *env,int isheredoc, int is_lim);
void ft_exection(t_data *data,t_env **env);
t_heredoc				*ft_hdocnew(int fd);
void					ft_hdocadd_back(t_heredoc **lst, t_heredoc *new);
char					*ft_itoa(int n);
int						ft_isalpha(int c);
int						ft_isdigit(int c);
void					rl_replace_line(char *,int);
void					sig_handler(int sig);
void					ft_echo(char **str);
void					ft_env(t_env *env,int is_export);
void					ft_export(t_env **env, char **str);
void					ft_unset(t_env *env, char *str);
char					*ft_search_env(t_env *env,char *str);
int						ft_search_inset(t_env *env,char **str,int equal,int add);
int						ft_check_error(t_list *list);
int ft_full_data(t_list *list,t_data **data,t_env *env);

#endif