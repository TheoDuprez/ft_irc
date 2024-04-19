# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: acarlott <acarlott@student.42lyon.fr>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/03 11:48:15 by acarlott          #+#    #+#              #
#    Updated: 2024/04/19 04:14:58 by acarlott         ###   ########lyon.fr    #
#                                                                              #
# **************************************************************************** #

NAME = ircserv

CFLAGS = -std=c++98 -Wall -Wextra -Werror

CC = c++

HEADER_PATH = includes/

SRCS_DIR = srcs/

COMMANDS_DIR = srcs/commands/

OBJS_DIR = .objs/

SRCS_LIST =	main.cpp \
			Server.cpp \
			Client.cpp \
			Channel.cpp \
			utils.cpp \


COMMANDS_LIST = joinCommand.cpp \
				partCommand.cpp \
				modeCommand.cpp \
                privmsgCommand.cpp \
                passCommand.cpp \
                nickCommand.cpp \
                userCommand.cpp \
				errorCommand.cpp \
				kickCommand.cpp \
				topicCommand.cpp \
				inviteCommand.cpp \
				quitCommand.cpp \
				lusersCommand.cpp \
				motdCommand.cpp \
				pingCommand.cpp \
				pongCommand.cpp


SRCS = $(addprefix $(SRCS_DIR), $(SRCS_LIST))

SRCS += $(addprefix $(COMMANDS_DIR), $(COMMANDS_LIST))

OBJS = $(addprefix $(OBJS_DIR), ${SRCS:.cpp=.o})

BONUS			=	ircbot

BONUS_SRCS_DIR =	bonus/srcs/

BONUS_OBJS_DIR =	${OBJS_DIR}${BONUS_SRCS_DIR}

BONUS_SRCS_LIST	=	main.cpp \
					Bot.cpp

BONUS_SRCS		=	${addprefix ${BONUS_SRCS_DIR}, ${BONUS_SRCS_LIST}}

BONUS_OBJS		=	${addprefix ${OBJS_DIR}, ${BONUS_SRCS:.cpp=.o}}

$(OBJS_DIR)%.o:	%.cpp | $(OBJS_DIR)
	${CC} ${CFLAGS} -I ${HEADER_PATH} -MMD -c $< -o $@

$(BONUS_OBJS_DIR)%.o:	%.cpp | $(BONUS_OBJS_DIR)
	${CC} ${CFLAGS} -I ${HEADER_PATH} -MMD -c $< -o $@

all : ${NAME}

bonus	: ${BONUS_OBJS_DIR} ${BONUS}

${NAME} : ${OBJS}
	echo "\033[1;34mCreate: \033[1;33mircserv"
	${CC} ${CFLAGS} ${OBJS} -o ${NAME}
	echo "\033[1;32mSucces !"

${BONUS} : ${BONUS_OBJS}
	echo "\033[1;34mCreate: \033[1;33mircbot"
	${CC} ${CFLAGS} ${BONUS_OBJS} -o ${BONUS}
	echo "\033[1;32mSucces !"

clean :
	echo "\033[1;31mClean: \033[1;36mAll *.o"
	rm -rf ${OBJS_DIR}

fclean: clean
	echo "\033[1;31mClean: \033[1;36mexecutable"
	rm -rf ${NAME}
	rm -rf ${BONUS}

re : fclean all

$(OBJS_DIR):
	mkdir -p $(OBJS_DIR)$(SRCS_DIR) $(OBJS_DIR)$(COMMANDS_DIR)

${BONUS_OBJS_DIR}:
	mkdir -p ${BONUS_OBJS_DIR}

-include $(OBJS:.o=.d)

-include $(BONUS_OBJS:.o=.d)

.SILENT:

.PHONY: all clean fclean re bonus
