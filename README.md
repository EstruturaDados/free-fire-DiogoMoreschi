# 🔫🎒 Desafio Código da Ilha – Edição Free Fire

Bem-vindo ao **Desafio Código da Ilha – Edição Free Fire!**

Este repositório contém a implementação em C de um sistema de mochila virtual
dividido em três níveis de complexidade:

- **Novato**: inventário básico (adicionar, remover, listar)
- **Aventureiro**: busca sequencial por nome
- **Mestre**: ordenação (Insertion Sort) e busca binária por nome

O arquivo principal é `FreeFire.c` e foi escrito para ser compilado com `gcc`.

---

## Como compilar e executar

Instruções rápidas para compilar e rodar o programa em um sistema Linux com `gcc`:

Compilar:
```bash
gcc -std=c11 FreeFire.c -o freefire
```

Executar:
```bash
./freefire
```

Exemplo rápido (inicia o menu e sai):
```bash
printf "0\n" | ./freefire
```

Observações:

- A mochila tem capacidade estática de 10 itens.
- O programa implementa as funções: adicionar, remover, listar, busca sequencial,
  ordenação (Insertion Sort) e busca binária (requer ordenação por nome).
- O campo `prioridade` (1-5) é usado no nível Mestre para ordenação por prioridade.

---

Se quiser, posso adicionar um `Makefile`, exemplos de uso ou testes automatizados.

> Equipe de Ensino – MateCheck
