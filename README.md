# E-Wan : Éditeur de Texte

Un éditeur de texte en ligne de commande écrit en C++, inspiré de Vim.
Ce projet a été réalisé pour mieux comprendre les **flux d'entrée et de sortie** (`cin`, `cout`, `cerr`, `ifstream`, `ofstream`) en C++.

Une fois installé, il se lance comme Vim, depuis n'importe quel dossier :

```
E-wan notes.txt
```

---

## Sommaire

1. [Fonctionnalités](#fonctionnalités)
2. [Prérequis](#prérequis)
3. [Installation](#installation)
4. [Utilisation](#utilisation)
5. [Liste des commandes](#liste-des-commandes)
6. [Exemple de session](#exemple-de-session)
7. [Comment ça marche](#comment-ça-marche)
8. [Limites actuelles](#limites-actuelles)
9. [Dépannage](#dépannage)
10. [Idées d'évolution](#idées-dévolution)

---

## Fonctionnalités

- Ouvrir un fichier texte existant, ou en créer un nouveau
- Afficher le contenu du fichier avec les numéros de ligne
- Ajouter du texte ligne par ligne
- Sauvegarder à tout moment, ou à la sortie
- Recharger le fichier depuis le disque
- Quitter avec ou sans sauvegarde
- Message d'erreur sur les commandes inconnues (une faute de frappe n'est pas écrite dans le fichier)

---

## Prérequis

Un compilateur C++ (norme C++11 ou plus récente).

| Système | Compilateur conseillé |
|---------|-----------------------|
| Windows | [MSYS2 / MinGW-w64](https://www.msys2.org/) (`g++`) |
| Linux   | `sudo apt install g++` (Debian/Ubuntu) |
| macOS   | `xcode-select --install` (fournit `clang++`) |

Vérifie que le compilateur est bien installé :

```
g++ --version
```

---

## Installation

### 1. Récupérer le code

```
git clone https://github.com/EwanFRANCOIS/E-wan-Editeur-de-Texte.git
cd E-wan-Editeur-de-Texte
```

Ou télécharge simplement `main.cpp` depuis la page GitHub.

### 2. Compiler

**Windows (PowerShell)**

```powershell
g++ -std=c++17 -Wall -Wextra main.cpp -o E-wan.exe
```

**Linux / macOS**

```bash
g++ -std=c++17 -Wall -Wextra main.cpp -o E-wan
```

### 3. Tester sans installer

```powershell
.\E-wan.exe test.txt      # Windows
./E-wan test.txt          # Linux / macOS
```

### 4. Pouvoir le lancer de n'importe où

#### Windows

Crée un dossier pour tes outils et compile directement dedans :

```powershell
mkdir C:\Outils
g++ -std=c++17 main.cpp -o C:\Outils\E-wan.exe
```

Ajoute ce dossier à la variable d'environnement `Path` de ton compte (une seule fois) :

```powershell
[Environment]::SetEnvironmentVariable("Path", $env:Path + ";C:\Outils", "User")
```

**Ferme puis rouvre PowerShell** : les terminaux déjà ouverts ne voient pas le nouveau `Path`.

Vérification :

```powershell
E-wan test.txt
```

> Alternative graphique : touche Windows, « variables d'environnement », *Modifier les variables d'environnement pour votre compte*, sélectionne `Path`, **Modifier**, **Nouveau**, puis ajoute `C:\Outils`.

#### Linux / macOS

```bash
sudo cp E-wan /usr/local/bin/
```

Sans `sudo` : copie l'exécutable dans `~/.local/bin/` (vérifie que ce dossier est dans ton `PATH`).

### 5. Mettre à jour

À chaque modification du code, recompile (et recopie l'exécutable si tu ne compiles pas directement dans le dossier du `Path`).

---

## Utilisation

```
E-wan <nom_du_fichier>
```

- Si le fichier **existe**, son contenu est chargé et affiché.
- S'il **n'existe pas**, l'éditeur démarre avec un fichier vide. Le fichier ne sera créé sur le disque qu'au premier `:save` ou `:exitS!`.
- Sans argument, le programme affiche son mode d'emploi et s'arrête.

Ensuite, **chaque ligne que tu tapes (validée par Entrée)** est interprétée ainsi :

| Ce que tu tapes | Ce qui se passe |
|-----------------|-----------------|
| Une commande connue (`:save`, `:show`...) | La commande est exécutée |
| Du texte normal | La ligne est ajoutée **à la fin** du fichier |
| Un texte qui commence par `:` mais qui n'est pas une commande | Message « Commande inconnue » |

> Important : les modifications restent **en mémoire** tant que tu n'as pas tapé `:save` ou `:exitS!`.

---

## Liste des commandes

| Commande | Description |
|----------|-------------|
| `:help`   | Affiche l'aide |
| `:show`   | Affiche le texte avec les numéros de ligne |
| `:save`   | Sauvegarde le texte dans le fichier |
| `:load`   | Recharge le fichier depuis le disque (annule les modifications non sauvegardées) |
| `:exitS!` | Quitte **avec** sauvegarde |
| `:exit!`  | Quitte **sans** sauvegarder |

---

## Exemple de session

```
PS> E-wan notes.txt
Bienvenue dans E-Wan, l'éditeur de texte en C++ !
Entrez votre texte ci-dessous (tapez :help pour afficher l'aide)
Nouveau fichier : notes.txt
0 ligne(s) chargée(s) dans le fichier : notes.txt
(fichier vide)
Premier paragraphe de mes notes
Deuxième ligne
:show
1 | Premier paragraphe de mes notes
2 | Deuxième ligne
:save
Fichier sauvegardé avec succès : notes.txt
Une ligne que je veux finalement jeter
:exit!
Merci d'avoir utilisé E-Wan !

PS> type notes.txt
Premier paragraphe de mes notes
Deuxième ligne
```

La dernière ligne n'apparaît pas dans le fichier, car `:exit!` quitte sans sauvegarder.

---

## Comment ça marche

Le programme est volontairement simple :

1. **Lecture de l'argument** : le nom du fichier est récupéré dans `argv[1]`.
2. **Chargement** : le fichier est lu ligne par ligne (`ifstream` + `getline`) et stocké dans un `vector<string>`, une case par ligne.
3. **Boucle principale** : chaque ligne tapée au clavier est lue avec `getline(cin, input)`, puis comparée aux commandes connues.
4. **Sauvegarde** : le contenu du `vector` est réécrit dans le fichier avec un `ofstream`.

Fonctions du programme :

| Fonction | Rôle |
|----------|------|
| `saveFile` | Écrit les lignes dans le fichier |
| `loadFile` | Relit le fichier et remplace le contenu en mémoire |
| `showFile` | Affiche les lignes numérotées |
| `showHelp` | Affiche la liste des commandes |
| `main`     | Lit l'argument, charge le fichier, lance la boucle |

---

## Limites actuelles

- On ne peut qu'**ajouter** des lignes à la fin : pas de modification ni de suppression d'une ligne précise.
- Pas de déplacement du curseur avec les flèches, pas d'affichage plein écran (le programme lit des lignes entières validées par Entrée, pas des touches une par une).
- Pas d'annulation (undo).
- Une ligne de texte ne peut pas commencer par `:` (elle serait prise pour une commande).

---

## Dépannage

**`g++` n'est pas reconnu**
Le compilateur n'est pas installé, ou son dossier n'est pas dans le `Path`. Vérifie avec `g++ --version`.

**`E-wan` n'est pas reconnu**
Le dossier de l'exécutable n'est pas dans le `Path`, ou le terminal n'a pas été redémarré après la modification. Ferme-le complètement et rouvre-le. Pour vérifier :

```powershell
$env:Path -split ";" | Select-String Outils
```

**Les accents s'affichent mal sous Windows (`Ã©` au lieu de `é`)**
La console n'est pas en UTF-8. Lance cette commande avant l'éditeur :

```powershell
chcp 65001
```

**Erreur « impossible d'ouvrir le fichier pour la sauvegarde »**
Tu n'as pas les droits d'écriture dans le dossier courant. Place-toi dans un dossier qui t'appartient (Documents, Bureau...).

**`:exit!` a quand même sauvegardé**
Vérifie qu'il ne reste pas de bloc de sauvegarde après la boucle `while` dans `main`. Seuls `:save` et `:exitS!` doivent écrire dans le fichier.

---

## Idées d'évolution

- [ ] Commandes `:d <n>` (supprimer), `:e <n> <texte>` (modifier), `:i <n> <texte>` (insérer)
- [ ] Copier-coller de lignes (`:y`, `:p`)
- [ ] Recherche et remplacement (`:find`, `:replace`)
- [ ] Annulation (undo) avec une pile d'états
- [ ] Avertissement en cas de modifications non sauvegardées
- [ ] Interface plein écran avec curseur, modes normal/insertion et barre de statut (ncurses / PDCurses)

---

## Auteur

Projet personnel d'**Ewan FRANCOIS**, réalisé pour apprendre.
