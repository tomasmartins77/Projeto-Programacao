#include "headers.h"

/** \brief funcao que le o ficheiro e o coloca por completo numa lista, no caso de se ter
 *         escolhido a opcao -L "continente", cria a lista apenas para esse continente
 *
 * \param settings settings_t* definicoes para saber se a lista é ALL ou "continente"
 * \return lista_t* lista criada de acordo com os argumentos da linha de comando
 *
 */
lista_t *ler_ficheiro(settings_t *settings)
{
    FILE *ficheiro;
    lista_t *lista = cria_lista(); //cria lista vazia

    ficheiro = fopen(settings->criterio_file, settings->tipo_ficheiro); // abre o ficheiro
    if (ficheiro == NULL)                                               // verifica se ouve algum erro na abertura do ficheiro
    {
        fprintf(stderr, "Erro a abrir ficheiro\n");
        exit(EXIT_FAILURE);
    }

    if (strcmp(settings->tipo_ficheiro, "r") == 0)
    {
        ler_ficheiro_csv(settings, ficheiro, lista);
    }
    else
    {
        ler_ficheiro_dat(settings, ficheiro, lista);
    }

    fclose(ficheiro);
    return lista;
}

/** \brief
 *
 * \param settings settings_t*
 * \param file FILE*
 * \param lista lista_t*
 * \return void
 *
 */
void ler_ficheiro_csv(settings_t *settings, FILE *file, lista_t *lista)
{
    char buffer[MAX_PALAVRAS_LINHAS];
    int linhas = 0; // contador de linhas, para nao ler a primeira, pois e o cabecalho
    while (fgets(buffer, MAX_PALAVRAS_LINHAS, file) != NULL)
    {
        if (linhas == 0)
        {
            linhas++;
            continue; // primeira linha queremos ignorar
        }
        ler_linha(settings, lista, buffer);
    }
}

/** \brief
 *
 * \param settings settings_t*
 * \param file FILE*
 * \param lista lista_t*
 * \return void
 *
 */
void ler_ficheiro_dat(settings_t *settings, FILE *file, lista_t *lista)
{
    int count_paises;

    fread(&count_paises, sizeof(int), 1, file);

    for (; count_paises > 0; count_paises--)
    {
        int count, count_dados;

        pais_t *pais = cria_pais();

        fread(&count, sizeof(int), 1, file);
        pais->country = malloc(sizeof(char) * count);
        fread(pais->country, sizeof(char), count, file);

        fread(&count, sizeof(int), 1, file);
        pais->country_code = malloc(sizeof(char) * count);
        fread(pais->country_code, sizeof(char), count, file);

        fread(&count, sizeof(int), 1, file);
        pais->continent = malloc(sizeof(char) * count);
        fread(pais->continent, sizeof(char), count, file);

        fread(&pais->population, sizeof(int), 1, file);

        fread(&count_dados, sizeof(int), 1, file);
        pais->dados = cria_lista();

        for (; count_dados > 0; count_dados--)
        {
            dados_t *dados = malloc(sizeof(dados_t));

            fread(&count, sizeof(int), 1, file);
            dados->indicator = malloc(sizeof(char) * count);
            fread(dados->indicator, sizeof(char), count, file);

            fread(&dados->weekly_count, sizeof(int), 1, file);

            dados->year_week = malloc(sizeof(yearWeek_t));

            fread(&dados->year_week->year, sizeof(int), 1, file);

            fread(&dados->year_week->week, sizeof(int), 1, file);

            fread(&dados->rate_14_day, sizeof(double), 1, file);

            fread(&dados->cumulative_count, sizeof(int), 1, file);

            dados->pais = pais;
            inserir_elemento_final(pais->dados, dados);
        }
        inserir_elemento_final(lista, pais);
    }
}

/** \brief le a linha, divide-a em varias strings e coloca-as nas diferentes variaveis de um node
 *
 * \param letra char* linha lida do ficheiro
 * \return dados_t* node com os elementos da linha nas suas variaveis
 *
 */
void ler_linha(settings_t *settings, lista_t *lista, char *letra)
{
    char *inicio_coluna = letra;
    int coluna = 0, contador = 1;
    pais_t *pais = cria_pais();

    dados_t *dados = malloc(sizeof(dados_t));
    if (dados == NULL)
    {
        fprintf(stderr, "Erro a alocar memoria para dados");
        exit(EXIT_FAILURE);
    }

    while (*letra != '\0' && *letra != '\n')
    {
        if (*letra == ',') // troca as "," por "\0" para dividir a string em varias substrings
        {
            *letra = '\0';

            inserir_dados(pais, dados, inicio_coluna, coluna);

            if(erro_letra_em_numero(inicio_coluna, contador))
            {
                fprintf(stderr, "existem letras onde deviam existir apenas numeros");
                liberta_settings(settings);
                liberta_lista(lista, destruir_pais);
                exit(EXIT_FAILURE);
            }
            contador++;
            coluna++;
            inicio_coluna = letra + 1;
        }
        letra++;
    }
    if(contador != 9)//se tem falta de colunas
    {
        fprintf(stderr, "nao existem colunas suficientes para um ficheiro valido");
        liberta_settings(settings);
        liberta_lista(lista, destruir_pais);
        exit(EXIT_FAILURE);
    }

    *letra = '\0';
    inserir_dados(pais, dados, inicio_coluna, coluna);

    if (settings->criterio_leitura == L_CONTINENTE && strcmp(pais->continent, settings->leitura_continente) != 0)
    {
        //descartar linha
        destruir_pais(pais);
        destruir_dados(dados);
    }
    else
    {
        insere_pais_dados_lista(lista, pais, dados);
    }
}

/** \brief insere um determinado dado na variavel correta da lista
 *
 * \param dados dados_t* lista para onde vao os dados do ficheiro
 * \param inicio_coluna char* palavra que vai entrar em cada variavel
 * \param coluna int indica em que coluna estamos de modo a colocar o valor na variavel correta
 * \return void
 *
 */
void inserir_dados(pais_t *pais, dados_t *dados, char *inicio_coluna, int coluna)
{
    switch (coluna)
    {
    case 0:
        pais->country = malloc(sizeof(char) * (strlen(inicio_coluna) + 1));
        strcpy(pais->country, inicio_coluna);
        break;
    case 1:
        pais->country_code = malloc(sizeof(char) * (strlen(inicio_coluna) + 1));
        strcpy(pais->country_code, inicio_coluna);

        break;
    case 2:
        pais->continent = malloc(sizeof(char) * (strlen(inicio_coluna) + 1));
        strcpy(pais->continent, inicio_coluna);
        break;
    case 3:
        pais->population = atoi(inicio_coluna);
        break;
    case 4:
        dados->indicator = malloc(sizeof(char) * (strlen(inicio_coluna) + 1));
        strcpy(dados->indicator, inicio_coluna);
        break;
    case 5:
        dados->weekly_count = atoi(inicio_coluna);
        break;
    case 6:
        dados->year_week = parseYearWeek(inicio_coluna);
        break;
    case 7:
        dados->rate_14_day = atof(inicio_coluna);
        break;
    case 8:
        dados->cumulative_count = atoi(inicio_coluna);
        break;
    }
}

/** \brief cria um ficheiro
 *
 * \param root lista_t* lista que vai criar o ficheiro
 * \param settings settings_t* saber qual o nome do ficheiro
 * \return void
 *
 */
void cria_ficheiro(lista_t *root, settings_t *settings)
{
    FILE *fp;

    fp = fopen(settings->criterio_write, settings->tipo_escrita);
    if (fp == NULL)
    {
        fprintf(stderr, "Erro a criar ficheiro");
        exit(EXIT_FAILURE);
    }
    if (strcmp(settings->tipo_escrita, "w") == 0)
    {
        escreve_ficheiro_csv(root, fp);
    }
    else
    {
        escreve_ficheiro_dat(root, fp);
    }

    fclose(fp);
}

/** \brief
 *
 * \param paises lista_t*
 * \param file FILE*
 * \return void
 *
 */
void escreve_ficheiro_csv(lista_t *paises, FILE *file)
{
    fprintf(file, "country,country_code,continent,population,indicator,weekly_count,year_week,rate_14_day,cumulative_count\n");

    node_t *curr = paises->first;

    while (curr != NULL)
    {
        pais_t *pais = curr->value;

        node_t *curr_dados = pais->dados->first;

        while (curr_dados != NULL)
        {
            dados_t *dados = curr_dados->value;

            fprintf(file, "%s,%s,%s,%d,%s,%d,%04d-%02d,%f,%d\n", pais->country, pais->country_code,
                    pais->continent, pais->population, dados->indicator, dados->weekly_count,
                    dados->year_week->year, dados->year_week->week, dados->rate_14_day, dados->cumulative_count);

            curr_dados = curr_dados->next;
        }

        curr = curr->next;
    }
}

/** \brief
 *
 * \param paises lista_t*
 * \param file FILE*
 * \return void
 *
 */
void escreve_ficheiro_dat(lista_t *paises, FILE *file)
{
    node_t *curr = paises->first;

    int count_paises = tamanho_lista(paises);

    fwrite(&count_paises, sizeof(int), 1, file);

    while (curr != NULL)
    {
        pais_t *pais = curr->value;
        int count_dados = tamanho_lista(pais->dados);
        int count;

        if (count_dados == 0)
            continue;

        count = strlen(pais->country) + 1;
        fwrite(&count, sizeof(int), 1, file);
        fwrite(pais->country, sizeof(char), count, file);
        count = strlen(pais->country_code) + 1;
        fwrite(&count, sizeof(int), 1, file);
        fwrite(pais->country_code, sizeof(char), count, file);
        count = strlen(pais->continent) + 1;
        fwrite(&count, sizeof(int), 1, file);
        fwrite(pais->continent, sizeof(char), count, file);

        fwrite(&pais->population, sizeof(int), 1, file);

        // escrever dados
        fwrite(&count_dados, sizeof(int), 1, file);

        node_t *curr_dados = pais->dados->first;
        while (curr_dados != NULL)
        {
            dados_t *dados = curr_dados->value;

            count = strlen(dados->indicator) + 1;
            fwrite(&count, sizeof(int), 1, file);

            fwrite(dados->indicator, sizeof(char), count, file);

            fwrite(&dados->weekly_count, sizeof(int), 1, file);

            fwrite(&dados->year_week->year, sizeof(int), 1, file);

            fwrite(&dados->year_week->week, sizeof(int), 1, file);

            fwrite(&dados->rate_14_day, sizeof(double), 1, file);

            fwrite(&dados->cumulative_count, sizeof(int), 1, file);

            curr_dados = curr_dados->next;
        }
        curr = curr->next;
    }
}
