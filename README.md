** Manipulação e Organização de Arquivos de Dados**
 Em sistemas de gerenciamento de banco de dados e aplicações que manipulam grandes
 volumes de informação, a forma como os dados são armazenados fisicamente em disco
 influencia diretamente o desempenho e a eficiência do sistema.
 Neste trabalho, você deverá desenvolver um programa que simule a persistência de
 registros em um arquivo de dados (.DAT), considerando as restrições de armazena
mento em blocos e diferentes estratégias de organização de registros.
 Oobjetivo é compreender as implicações práticas das decisões de projeto relacionadas
 ao tamanho dos registros, limitação de blocos e métodos de alocação de dados
 em disco.
 
** Objetivos**
 • Implementar a leitura e gravação de registros em arquivos binários (.DAT);
 • Simular o armazenamento de registros em blocos de tamanho fixo (em bytes),
 especificado pelo usuário;
 • Avaliar o impacto das diferentes estratégias de organização:
 1. Registros de tamanho fixo;
 2. Registros de tamanho variável, com duas variações:
 (a) Contíguos (sem espalhamento);
 (b) Espalhados (fragmentados entre blocos).
 • Calcular e exibir estatísticas de uso dos blocos e eficiência de armazenamento
