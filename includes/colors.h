/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colors.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldalmass <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 12:18:44 by hugolefevre       #+#    #+#             */
/*   Updated: 2026/11/05 18:00:09 by ldalmass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COLORS_H
#define COLORS_H

#include "./tui.h"

#define RESET "\033[0m"
#define WHITE "\033[37m"
#define BLACK "\033[30m"
#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define YELLOW "\033[0;33m"
#define BLUE "\033[0;34m"
#define MAGENTA "\033[0;35m"
#define CYAN "\033[0;36m"

#define BG_BLACK "\033[40m"
#define BG_RED "\033[41m"
#define BG_GREEN "\033[42m"
#define BG_YELLOW "\033[43m"
#define BG_BLUE "\033[44m"
#define BG_MAGENTA "\033[45m"
#define BG_CYAN "\033[46m"
#define BG_WHITE "\033[47m"

#define INFO WHITE CAP_ROUND_L BG_WHITE BLACK " INFO " RESET WHITE CAP_ROUND_R RESET
#define DEBUG BLUE CAP_ROUND_L BG_BLUE BLACK	 " DEBUG " RESET BLUE CAP_ROUND_R RESET
#define WARNING YELLOW CAP_ROUND_L BG_YELLOW " WARNING " RESET YELLOW CAP_ROUND_R RESET
#define ERROR RED CAP_ROUND_L BG_RED " ERROR " RESET RED CAP_ROUND_R RESET

#define PASSES GREEN CAP_ROUND_L BG_GREEN BLACK " PASSES " RESET GREEN CAP_ROUND_R RESET
#define PASS GREEN CAP_ROUND_L BG_GREEN BLACK " PASS " RESET GREEN CAP_ROUND_R RESET
#define FAILED RED CAP_ROUND_L BG_RED WHITE " FAILED " RESET RED CAP_ROUND_R RESET
#define FAIL RED CAP_ROUND_L BG_RED WHITE " FAIL " RESET RED CAP_ROUND_R RESET

#define BOOL_STR(boolean) ((boolean) ? GREEN CAP_ROUND_L BG_GREEN BLACK "true" RESET GREEN CAP_ROUND_R RESET : RED CAP_ROUND_L BG_RED WHITE "false" RESET RED CAP_ROUND_R RESET)
// #define BOOL_STR(boolean) ((boolean) ? "true" : "false")

#endif