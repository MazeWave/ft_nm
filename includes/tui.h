/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tui.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldalmass <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/05 18:00:00 by ldalmass          #+#    #+#             */
/*   Updated: 2026/11/05 18:00:00 by ldalmass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TUI_H
#define TUI_H

/*
** UTF-8 glyphs for terminal UI, written as byte escapes so they work
** with any C standard. Glyph, codepoint and notes are in each comment.
**
** Badge with rounded caps: cap fg colour == badge bg colour, e.g.
**   GREEN CAP_ROUND_L BG_GREEN BLACK " PASS " RESET GREEN CAP_ROUND_R RESET
*/

/* Powerline rounded caps (Nerd Font; kitty draws them natively, full cell) */
#define CAP_ROUND_L        "\xee\x82\xb6"    /*   U+E0B6  left half circle, filled */
#define CAP_ROUND_R        "\xee\x82\xb4"    /*   U+E0B4  right half circle, filled */
#define CAP_ROUND_L_THIN   "\xee\x82\xb7"    /*   U+E0B7  left half circle, outline */
#define CAP_ROUND_R_THIN   "\xee\x82\xb5"    /*   U+E0B5  right half circle, outline */

/* Powerline arrows & slants (Nerd Font) */
#define CAP_ARROW_R        "\xee\x82\xb0"    /*   U+E0B0  right-pointing triangle */
#define CAP_ARROW_L        "\xee\x82\xb2"    /*   U+E0B2  left-pointing triangle */
#define CAP_ARROW_R_THIN   "\xee\x82\xb1"    /*   U+E0B1  right chevron, outline */
#define CAP_ARROW_L_THIN   "\xee\x82\xb3"    /*   U+E0B3  left chevron, outline */
#define CAP_SLANT_LL       "\xee\x82\xb8"    /*   U+E0B8  lower-left triangle */
#define CAP_SLANT_LR       "\xee\x82\xba"    /*   U+E0BA  lower-right triangle */
#define CAP_SLANT_UL       "\xee\x82\xbc"    /*   U+E0BC  upper-left triangle */
#define CAP_SLANT_UR       "\xee\x82\xbe"    /*   U+E0BE  upper-right triangle */

/* Half circles, legacy computing (half-cell wide, NOT full cell) */
#define HALF_CIRCLE_TOP    "\xf0\x9f\xaf\xa8"/* 🯨  U+1FBE8  top-justified lower half */
#define HALF_CIRCLE_RIGHT  "\xf0\x9f\xaf\xa9"/* 🯩  U+1FBE9  right-justified left half */
#define HALF_CIRCLE_BOTTOM "\xf0\x9f\xaf\xaa"/* 🯪  U+1FBEA  bottom-justified upper half */
#define HALF_CIRCLE_LEFT   "\xf0\x9f\xaf\xab"/* 🯫  U+1FBEB  left-justified right half */
#define HALF_CIRCLE_L      "\xe2\x97\x96"    /* ◖  U+25D6  left half black circle */
#define HALF_CIRCLE_R      "\xe2\x97\x97"    /* ◗  U+25D7  right half black circle */

/* Box drawing: light */
#define BOX_H              "\xe2\x94\x80"    /* ─  U+2500 */
#define BOX_V              "\xe2\x94\x82"    /* │  U+2502 */
#define BOX_TL             "\xe2\x94\x8c"    /* ┌  U+250C */
#define BOX_TR             "\xe2\x94\x90"    /* ┐  U+2510 */
#define BOX_BL             "\xe2\x94\x94"    /* └  U+2514 */
#define BOX_BR             "\xe2\x94\x98"    /* ┘  U+2518 */
#define BOX_T              "\xe2\x94\xac"    /* ┬  U+252C  tee down */
#define BOX_B              "\xe2\x94\xb4"    /* ┴  U+2534  tee up */
#define BOX_L              "\xe2\x94\x9c"    /* ├  U+251C  tee right */
#define BOX_R              "\xe2\x94\xa4"    /* ┤  U+2524  tee left */
#define BOX_X              "\xe2\x94\xbc"    /* ┼  U+253C  cross */

/* Box drawing: rounded corners (use light H/V for edges) */
#define BOX_ROUND_TL       "\xe2\x95\xad"    /* ╭  U+256D */
#define BOX_ROUND_TR       "\xe2\x95\xae"    /* ╮  U+256E */
#define BOX_ROUND_BL       "\xe2\x95\xb0"    /* ╰  U+2570 */
#define BOX_ROUND_BR       "\xe2\x95\xaf"    /* ╯  U+256F */

/* Box drawing: heavy */
#define BOX_HEAVY_H        "\xe2\x94\x81"    /* ━  U+2501 */
#define BOX_HEAVY_V        "\xe2\x94\x83"    /* ┃  U+2503 */
#define BOX_HEAVY_TL       "\xe2\x94\x8f"    /* ┏  U+250F */
#define BOX_HEAVY_TR       "\xe2\x94\x93"    /* ┓  U+2513 */
#define BOX_HEAVY_BL       "\xe2\x94\x97"    /* ┗  U+2517 */
#define BOX_HEAVY_BR       "\xe2\x94\x9b"    /* ┛  U+251B */
#define BOX_HEAVY_T        "\xe2\x94\xb3"    /* ┳  U+2533 */
#define BOX_HEAVY_B        "\xe2\x94\xbb"    /* ┻  U+253B */
#define BOX_HEAVY_L        "\xe2\x94\xa3"    /* ┣  U+2523 */
#define BOX_HEAVY_R        "\xe2\x94\xab"    /* ┫  U+252B */
#define BOX_HEAVY_X        "\xe2\x95\x8b"    /* ╋  U+254B */

/* Box drawing: double */
#define BOX_DBL_H          "\xe2\x95\x90"    /* ═  U+2550 */
#define BOX_DBL_V          "\xe2\x95\x91"    /* ║  U+2551 */
#define BOX_DBL_TL         "\xe2\x95\x94"    /* ╔  U+2554 */
#define BOX_DBL_TR         "\xe2\x95\x97"    /* ╗  U+2557 */
#define BOX_DBL_BL         "\xe2\x95\x9a"    /* ╚  U+255A */
#define BOX_DBL_BR         "\xe2\x95\x9d"    /* ╝  U+255D */
#define BOX_DBL_T          "\xe2\x95\xa6"    /* ╦  U+2566 */
#define BOX_DBL_B          "\xe2\x95\xa9"    /* ╩  U+2569 */
#define BOX_DBL_L          "\xe2\x95\xa0"    /* ╠  U+2560 */
#define BOX_DBL_R          "\xe2\x95\xa3"    /* ╣  U+2563 */
#define BOX_DBL_X          "\xe2\x95\xac"    /* ╬  U+256C */

/* Dashed lines (light) */
#define DASH2_H            "\xe2\x95\x8c"    /* ╌  U+254C  double dash */
#define DASH3_H            "\xe2\x94\x84"    /* ┄  U+2504  triple dash */
#define DASH4_H            "\xe2\x94\x88"    /* ┈  U+2508  quadruple dash */
#define DASH2_V            "\xe2\x95\x8e"    /* ╎  U+254E */
#define DASH3_V            "\xe2\x94\x86"    /* ┆  U+2506 */
#define DASH4_V            "\xe2\x94\x8a"    /* ┊  U+250A */

/* Line ends / half lines */
#define LINE_END_L         "\xe2\x95\xb4"    /* ╴  U+2574  left half */
#define LINE_END_U         "\xe2\x95\xb5"    /* ╵  U+2575  up half */
#define LINE_END_R         "\xe2\x95\xb6"    /* ╶  U+2576  right half */
#define LINE_END_D         "\xe2\x95\xb7"    /* ╷  U+2577  down half */

/* Blocks & shades */
#define BLOCK_FULL         "\xe2\x96\x88"    /* █  U+2588 */
#define BLOCK_UPPER        "\xe2\x96\x80"    /* ▀  U+2580  upper half */
#define BLOCK_LOWER        "\xe2\x96\x84"    /* ▄  U+2584  lower half */
#define BLOCK_LEFT         "\xe2\x96\x8c"    /* ▌  U+258C  left half */
#define BLOCK_RIGHT        "\xe2\x96\x90"    /* ▐  U+2590  right half */
#define SHADE_LIGHT        "\xe2\x96\x91"    /* ░  U+2591 */
#define SHADE_MEDIUM       "\xe2\x96\x92"    /* ▒  U+2592 */
#define SHADE_DARK         "\xe2\x96\x93"    /* ▓  U+2593 */

/* Progress bar eighths (left to right) */
#define BAR_1_8            "\xe2\x96\x8f"    /* ▏  U+258F */
#define BAR_2_8            "\xe2\x96\x8e"    /* ▎  U+258E */
#define BAR_3_8            "\xe2\x96\x8d"    /* ▍  U+258D */
#define BAR_4_8            "\xe2\x96\x8c"    /* ▌  U+258C */
#define BAR_5_8            "\xe2\x96\x8b"    /* ▋  U+258B */
#define BAR_6_8            "\xe2\x96\x8a"    /* ▊  U+258A */
#define BAR_7_8            "\xe2\x96\x89"    /* ▉  U+2589 */
#define BAR_8_8            "\xe2\x96\x88"    /* █  U+2588 */

/* Arrows */
#define ARROW_L            "\xe2\x86\x90"    /* ←  U+2190 */
#define ARROW_U            "\xe2\x86\x91"    /* ↑  U+2191 */
#define ARROW_R            "\xe2\x86\x92"    /* →  U+2192 */
#define ARROW_D            "\xe2\x86\x93"    /* ↓  U+2193 */
#define ARROW_RET          "\xe2\x86\xb3"    /* ↳  U+21B3  down then right */
#define TRI_R              "\xe2\x96\xb6"    /* ▶  U+25B6 */
#define TRI_D              "\xe2\x96\xbc"    /* ▼  U+25BC */
#define TRI_R_SMALL        "\xe2\x96\xb8"    /* ▸  U+25B8 */
#define TRI_D_SMALL        "\xe2\x96\xbe"    /* ▾  U+25BE */

/* Bullets & status */
#define BULLET             "\xe2\x80\xa2"    /* •  U+2022 */
#define DOT_MIDDLE         "\xc2\xb7"        /* ·  U+00B7 */
#define CIRCLE_FULL        "\xe2\x97\x8f"    /* ●  U+25CF */
#define CIRCLE_EMPTY       "\xe2\x97\x8b"    /* ○  U+25CB */
#define DIAMOND            "\xe2\x97\x86"    /* ◆  U+25C6 */
#define SQUARE_FULL        "\xe2\x96\xa0"    /* ■  U+25A0 */
#define SQUARE_EMPTY       "\xe2\x96\xa1"    /* □  U+25A1 */
#define CHECK              "\xe2\x9c\x94"    /* ✔  U+2714 */
#define CROSS              "\xe2\x9c\x98"    /* ✘  U+2718 */
#define WARN               "\xe2\x9a\xa0"    /* ⚠  U+26A0 */
#define ELLIPSIS           "\xe2\x80\xa6"    /* …  U+2026 */

/* Spinner frames (braille) */
#define SPIN_0             "\xe2\xa0\x8b"    /* ⠋  U+280B */
#define SPIN_1             "\xe2\xa0\x99"    /* ⠙  U+2819 */
#define SPIN_2             "\xe2\xa0\xb9"    /* ⠹  U+2839 */
#define SPIN_3             "\xe2\xa0\xb8"    /* ⠸  U+2838 */
#define SPIN_4             "\xe2\xa0\xbc"    /* ⠼  U+283C */
#define SPIN_5             "\xe2\xa0\xb4"    /* ⠴  U+2834 */
#define SPIN_6             "\xe2\xa0\xa6"    /* ⠦  U+2826 */
#define SPIN_7             "\xe2\xa0\xa7"    /* ⠧  U+2827 */
#define SPIN_8             "\xe2\xa0\x87"    /* ⠇  U+2807 */
#define SPIN_9             "\xe2\xa0\x8f"    /* ⠏  U+280F */

#endif
