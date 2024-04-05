# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: acarlott <acarlott@student.42lyon.fr>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/03 11:48:15 by acarlott          #+#    #+#              #
#    Updated: 2024/04/04 22:51:18 by acarlott         ###   ########lyon.fr    #
#                                                                              #
# **************************************************************************** #

NAME = ircserv

CFLAGS = -std=c++98 -Wall -Wextra -Werror

CC = c++

HEADER_PATH = includes/

SRCS_DIR = srcs/

CLASS_DIR = class/

OBJS_DIR = .objs/

SRCS_LIST =	main.cpp \

CLASS_LIST =	Server.cpp \
				Client.cpp \

SRCS = $(addprefix $(SRCS_DIR), $(SRCS_LIST))

SRCS += $(addprefix $(CLASS_DIR), $(CLASS_LIST))

OBJS = $(addprefix $(OBJS_DIR), ${SRCS:.cpp=.o})

$(OBJS_DIR)%.o:	%.cpp | $(OBJS_DIR)
	${CC} ${CFLAGS} -I ${HEADER_PATH} -MMD -c $< -o $@

all : ${NAME}

${NAME} : ${OBJS}
	echo "\033[1;34mCreate: \033[1;33mircserv"
	${CC} ${CFLAGS} ${OBJS} -o ${NAME}
	echo "\033[1;32mSucces !"

clean :
	echo "\033[1;31mClean: \033[1;36mAll *.o"
	rm -rf ${OBJS_DIR}

fclean: clean
	echo "\033[1;31mClean: \033[1;36mexecutable"
	rm -rf ${NAME}

re : fclean all

$(OBJS_DIR):
	mkdir -p $(OBJS_DIR)$(SRCS_DIR) $(OBJS_DIR)$(CLASS_DIR)

-include $(OBJS:.o=.d)

.SILENT:

.PHONY: all clean fclean re