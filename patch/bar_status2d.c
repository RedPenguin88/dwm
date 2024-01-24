int
width_status2d(Bar *bar, BarWidthArg *a)
{
	return status2dtextlength(rawstext) + lrpad;
}

int
width_status2d_es(Bar *bar, BarWidthArg *a)
{
	return status2dtextlength(rawestext);
}

int
draw_status2d(Bar *bar, BarDrawArg *a)
{
	return drawstatusbar(a->x, rawstext);
}

int
draw_status2d_es(Bar *bar, BarDrawArg *a)
{
	return drawstatusbar(a->x, rawestext);
}

int
drawstatusbar(int x, char* stext)
{
	int i, w, len;
	short isCode = 0;
	char *text;
	char *p;

	len = strlen(stext);
	if (!(text = (char*) malloc(sizeof(char)*(len + 1))))
		die("malloc");
	p = text;
	copyvalidchars(text, stext);
	text[len] = '\0';

	x += lrpad / 2;
	drw_setscheme(drw, scheme[LENGTH(colors)]);
	drw->scheme[ColFg] = scheme[SchemeNorm][ColFg];
	drw->scheme[ColBg] = scheme[SchemeNorm][ColBg];

	/* process status text */
	i = -1;
	while (text[++i]) {
		if (text[i] == '^' && !isCode) {
			isCode = 1;

			text[i] = '\0';
			w = TEXTW(text) - lrpad;
			drw_text(drw, x, 0, w, bh, 0, text, 0);

			x += w;

			/* process code */
			while (text[++i] != '^') {
				if (text[i] == 'c') {
					char buf[8];
					if (i + 7 > len - 1) {
						i += 7;
						len = 0;
						break;
					}
					memcpy(buf, (char*)text+i+1, 7);
					buf[7] = '\0';
					#if BAR_ALPHA_PATCH && BAR_STATUS2D_NO_ALPHA_PATCH
					drw_clr_create(drw, &drw->scheme[ColFg], buf, 0xff);
					#elif BAR_ALPHA_PATCH
					drw_clr_create(drw, &drw->scheme[ColFg], buf, alphas[SchemeNorm][ColFg]);
					#else
					drw_clr_create(drw, &drw->scheme[ColFg], buf);
					#endif // BAR_ALPHA_PATCH
					i += 7;
				} else if (text[i] == 'b') {
					char buf[8];
					if (i + 7 > len - 1) {
						i += 7;
						len = 0;
						break;
					}
					memcpy(buf, (char*)text+i+1, 7);
					buf[7] = '\0';
					#if BAR_ALPHA_PATCH && BAR_STATUS2D_NO_ALPHA_PATCH
					drw_clr_create(drw, &drw->scheme[ColBg], buf, 0xff);
					#elif BAR_ALPHA_PATCH
					drw_clr_create(drw, &drw->scheme[ColBg], buf, alphas[SchemeNorm][ColBg]);
					#else
					drw_clr_create(drw, &drw->scheme[ColBg], buf);
					#endif // BAR_ALPHA_PATCH
					i += 7;
				} else if (text[i] == 'd') {
					drw->scheme[ColFg] = scheme[SchemeNorm][ColFg];
					drw->scheme[ColBg] = scheme[SchemeNorm][ColBg];
				} else if (text[i] == 'r') {
					int rx = atoi(text + ++i);
					while (text[++i] != ',');
					int ry = atoi(text + ++i);
					while (text[++i] != ',');
					int rw = atoi(text + ++i);
					while (text[++i] != ',');
					int rh = atoi(text + ++i);

					if (ry < 0)
						ry = 0;
					if (rx < 0)
						rx = 0;

					drw_rect(drw, rx + x, ry, rw, rh, 1, 0);
				} else if (text[i] == 'f') {
					x += atoi(text + ++i);
				}
			}

			text = text + i + 1;
			len -= i + 1;
			i=-1;
			isCode = 0;
		}
	}
	if (!isCode && len) {
		w = TEXTW(text) - lrpad;
		drw_text(drw, x, 0, w, bh, 0, text, 0);
		x += w;
	}
	free(p);

	drw_setscheme(drw, scheme[SchemeNorm]);

	return x;
}

int
status2dtextlength(char* stext)
{
	int i, w, len;
	short isCode = 0;
	char *text;
	char *p;

	len = strlen(stext) + 1;
	if (!(text = (char*) malloc(sizeof(char)*len)))
		die("malloc");
	p = text;
	copyvalidchars(text, stext);

	/* compute width of the status text */
	w = 0;
	i = -1;
	while (text[++i]) {
		if (text[i] == '^') {
			if (!isCode) {
				isCode = 1;
				text[i] = '\0';
				w += TEXTW(text) - lrpad;
				text[i] = '^';
				if (text[++i] == 'f')
					w += atoi(text + ++i);
			} else {
				isCode = 0;
				text = text + i + 1;
				i = -1;
			}
		}
	}
	if (!isCode)
		w += TEXTW(text) - lrpad;
	free(p);
	return w;
}
