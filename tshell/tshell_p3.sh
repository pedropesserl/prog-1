#!/bin/bash

if [ $# -ne 2 ]; then
	>&2 echo "Uso: ./tshell_p3.sh <axml> <acsv>"
	exit 1
fi

confere_arq_entrada() {
	IFS=. read -r NOME EXT <<< $1
	if [ $EXT != "xml.gz" ]; then
		>&2 echo "Erro: formato incorreto para $1 (deve ser .xml.gz)"
		exit 2
	fi
	if [ ! -f $1 ]; then
		>&2 echo "Erro: $1 não existe"
		exit 3
	fi
}

confere_arq_saida() {
	IFS=. read -r NOME EXT <<< $1
	if [ $EXT != "csv" ]; then
		>&2 echo "Erro: formato incorreto para $1 (deve ser .csv)"
		exit 2
	fi
	
	echo "version+pmid<title<abstract<keywords" > $1
}

XML=$1
confere_arq_entrada $XML
XML=$(realpath $XML)

CSV=$2
confere_arq_saida $CSV
CSV=$(realpath $CSV)

# converter o arquivo xml.gz para o formato csv
zcat $XML | \
	xgrep -tx "//PMID|//ArticleTitle|//Abstract|//MeshHeadingList" | \
	# o sed acha os campos úteis -- PMID seguido de ArticleTitle seguido
	# de Abstract, seguido ou não de MeshHeadingList -- e converte no
	# formato csv. No caso de não haver MeshHeadingList para um dado artigo,
	# cria um campo vazio (<).
	sed -En '
	/PMID/{
		s/<PMID Version="//;
		s/">//;
		s/<\/PMID>//;
		N;

		/ArticleTitle/{
			s/<ArticleTitle>/</;
			s/<\/ArticleTitle>//;
			N;

			/Abstract/{
				s/<Abstract> *<AbstractText>/</;
				s/<\/AbstractText> *<\/Abstract>//;
				p;
				n;

				/MeshHeadingList/{
					s/ *<MeshHeading> *<Descriptor[^>]*>//g;
					s/(<\/DescriptorName>|<\/QualifierName>) *(<Qualifier[^>]*>|<\/MeshHeading>)/, /g;
					s/<MeshHeadingList>/</;
					s/(, *|)<\/MeshHeadingList>//;
					p;
				};

				/PMID/{
					i <
				}
			}
		}
	}' >> $CSV

# converter para o formato csv
# sed -i '
# 	/PMID/{
# 		s/<PMID Version="//;
# 		s/">//;
# 		s/<\/PMID>//;
# 	}

# 	/ArticleTitle/{
# 		s/<ArticleTitle>/</;
# 		s/<\/ArticleTitle>//;
# 	}

# 	/Abstract/{
# 		s/<Abstract> *<AbstractText>/</;
# 		s/<\/AbstractText> *<\/Abstract>//;
# 	}

# 	/MeshHeadingList/{
# 		s/ *<MeshHeading> *<Descriptor[^>]*>//g;
# 		s/\(<\/DescriptorName>\|<\/QualifierName>\) *\(<Qualifier[^>]*>\|<\/MeshHeading>\)/, /g;
# 		s/<MeshHeadingList>/</;
# 		s/\(, *\|\)<\/MeshHeadingList>//;
# 	}' $CSV

