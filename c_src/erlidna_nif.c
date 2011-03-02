#include <string.h>
#include <stdio.h>

#include <erl_nif.h>
#include <idna.h>

ERL_NIF_TERM erlidna_encode(ErlNifEnv* env, int argc,
    const ERL_NIF_TERM argv[]);
ERL_NIF_TERM erlidna_decode(ErlNifEnv* env, int argc,
    const ERL_NIF_TERM argv[]);

static void make_tag(ErlNifEnv* env, const char *str, ERL_NIF_TERM *atom);
static int prepare_data(ErlNifEnv* env, ERL_NIF_TERM bin_term, void **data);
static int prepare_resp(ErlNifEnv* env, int status, const char *s,
    ERL_NIF_TERM *resp);

static ErlNifFunc nif_funcs[] =
{
	{"encode", 1, erlidna_encode},
	{"decode", 1, erlidna_decode}
};

ERL_NIF_TERM
erlidna_encode(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
	char *encoded;
	void *data;
	int   status;

	ERL_NIF_TERM result;

	data = NULL;
	encoded = NULL;

	if (!prepare_data(env, argv[0], &data)) {
		result = enif_make_badarg(env);
	} else {
		status = idna_to_ascii_8z((const char*)data, &encoded, 0);
		if (!prepare_resp(env, status, encoded, &result)) {
			result = enif_make_badarg(env);
		}
	}
	
	free(data);
	free(encoded);

	return result;
}

ERL_NIF_TERM
erlidna_decode(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
	char *decoded;
	void *data;
	int   status;

	ERL_NIF_TERM result;

	data = NULL;
	decoded = NULL;

	if (!prepare_data(env, argv[0], &data)) {
		result = enif_make_badarg(env);
	} else {
		status = idna_to_unicode_8z8z((const char*)data, &decoded, 0);
		if (!prepare_resp(env, status, decoded, &result)) {
			result = enif_make_badarg(env);
		}
	}
	
	free(data);
	free(decoded);

	return result;
}

ERL_NIF_INIT(erlidna, nif_funcs, NULL, NULL, NULL, NULL);

int
prepare_resp(ErlNifEnv* env, int status, const char *s, ERL_NIF_TERM *resp)
{
	ERL_NIF_TERM tag;
	ErlNifBinary bin;

	if (status == IDNA_SUCCESS) {
		if (!enif_alloc_binary(strlen(s), &bin)) {
			return 0;
		}
		make_tag(env, "ok", &tag);
		memcpy(bin.data, s, bin.size);
		*resp = enif_make_tuple2(env, tag, enif_make_binary(env, &bin));
	} else {
		make_tag(env, "error", &tag);
		*resp = enif_make_tuple2(env, tag,
		    enif_make_string(env, idna_strerror(status),
			ERL_NIF_LATIN1));
	}

	return 1;
}

int
prepare_data(ErlNifEnv* env, ERL_NIF_TERM bin_term, void **data)
{
	ErlNifBinary bin;

	if (!enif_is_binary(env, bin_term)) {
		return 0;
	}

	if (!enif_inspect_binary(env, bin_term, &bin)) {
		return 0;
	}

	*data = calloc(1, bin.size + 1);

	if (*data == NULL) {
		return 0;
	}

	memcpy(*data, bin.data, bin.size);

	return 1;
}

void
make_tag(ErlNifEnv* env, const char *str, ERL_NIF_TERM *atom)
{
	if (!enif_make_existing_atom(env, str, atom, ERL_NIF_LATIN1)) {
		*atom = enif_make_atom(env, str);
	}
}
