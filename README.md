# Commande de rétroviseurs par bus CAN

Projet de fin d'études (BTS Systèmes Électroniques) : une interface **bus CAN** pour commander à distance les rétroviseurs électriques d'une voiture, à l'aide de deux cartes Arduino Uno et de modules MCP2515.

> 🎓 Lycée technique de Fès, filière Systèmes Électroniques, 2024
> 📄 Le rapport complet (66 pages) est disponible dans [`docs/rapport-PFE.pdf`](docs/rapport-PFE.pdf).

## Principe

Dans une voiture, les commandes et les actionneurs communiquent via un bus CAN. Ce projet reproduit ce fonctionnement à petite échelle avec deux nœuds :

```
 ┌───────────────────┐      Bus CAN (125 kbit/s)      ┌───────────────────┐
 │  ARD1 : Commande  │ ─────────  CAN H / CAN L ────► │  ARD2 : Récepteur │
 │  Arduino Uno      │        trame ID = 0x101        │  Arduino Uno      │
 │  + MCP2515        │                                │  + MCP2515        │
 │  + LCD 20x4       │                                │  + 2 x L298       │
 │  + 4 boutons      │                                │  + 2 moteurs DC   │
 │  + sélecteur G/D  │                                │    (rétroviseur)  │
 └───────────────────┘                                └───────────────────┘
```

- **ARD1 (`CAN_write`)** : lit les boutons *haut / bas / gauche / droite* et le sélecteur *rétroviseur gauche / droit*, affiche l'état sur un écran LCD 20x4, puis envoie une trame CAN.
- **ARD2 (`CAN_read`)** : reçoit la trame, la décode et pilote les deux moteurs DC du rétroviseur via un module L298.

Le rétroviseur utilisé pour les essais est un rétroviseur électrique **Dacia Logan** (côté passager, 2004-2012).

## Protocole CAN

| Paramètre | Valeur |
|---|---|
| Débit | 125 kbit/s |
| Identifiant | `0x101` |
| Longueur (DLC) | 1 octet |
| Octet `data[0]` | commande (voir tableau) |

| Commande | Rétroviseur gauche | Rétroviseur droit |
|---|:---:|:---:|
| Haut | `0x01` | `0x05` |
| Bas | `0x02` | `0x06` |
| Gauche | `0x03` | `0x07` |
| Droite | `0x04` | `0x08` |

## Matériel

- 2 x Arduino Uno
- 2 x module CAN MCP2515 (TJA1050)
- 1 x écran LCD 20x4 (HD44780)
- 4 x boutons-poussoirs + 1 sélecteur 3 positions
- 1 x module driver moteur L298N
- 1 x rétroviseur électrique (2 moteurs DC)

## Branchements

**ARD1 (commande)**

| Élément | Broche |
|---|---|
| MCP2515 (CS) | D10 (+ SPI : D11, D12, D13) |
| LCD (RS, EN, D4 à D7) | A0, A1, A2, A3, A4, A5 |
| Boutons haut / bas / gauche / droite | D7, D6, D5, D4 |
| Sélecteur gauche / droit | D0, D1 |

**ARD2 (récepteur)**

| Élément | Broche |
|---|---|
| MCP2515 (CS) | D10 (+ SPI : D11, D12, D13) |
| Moteur A (enA, in1, in2) | D9, A0, A1 |
| Moteur B (enB, in3, in4) | D3, A2, A3 |

## Installation

1. Installer l'[Arduino IDE](https://www.arduino.cc/en/software).
2. Installer la bibliothèque **arduino-mcp2515** (`Croquis > Inclure une bibliothèque > Gérer les bibliothèques`, chercher *mcp2515* de autowp). La bibliothèque `LiquidCrystal` est déjà incluse.
3. Téléverser `CAN_write/CAN_write.ino` sur la carte ARD1.
4. Téléverser `CAN_read/CAN_read.ino` sur la carte ARD2.
5. Relier les deux modules MCP2515 : `CANH` avec `CANH`, `CANL` avec `CANL`, et masse commune.

## Utilisation

1. Alimenter les deux cartes.
2. Choisir le rétroviseur avec le sélecteur (gauche ou droit) : l'écran LCD affiche `Mirror Left` ou `Mirror Right`.
3. Appuyer sur un bouton directionnel : la trame est envoyée et le rétroviseur bouge pendant 500 ms.
4. Le moniteur série (115200 bauds) affiche les trames reçues sur ARD2 (ID, DLC, données).

## Structure du dépôt

```
retroviseur-can-bus/
├── CAN_write/CAN_write.ino   # Nœud émetteur (boutons + LCD)
├── CAN_read/CAN_read.ino     # Nœud récepteur (moteurs)
├── docs/rapport-PFE.pdf      # Rapport de projet
└── README.md
```

## Conception et outils

Simulation sous **Proteus 8**, schémas sous **Fritzing**, circuits imprimés sous **Eagle**, programmation avec l'**Arduino IDE**. Les schémas, le routage et les photos sont détaillés dans le rapport.

## Pistes d'amélioration

- Le récepteur ne traite pour l'instant que les commandes `0x01` à `0x04` (rétroviseur gauche). Les commandes `0x05` à `0x08` (rétroviseur droit) sont envoyées mais pas encore interprétées.
- Les broches D0 et D1 du sélecteur sont aussi celles de la liaison série (RX/TX) : les déplacer (par exemple sur D2 et D3) évite les conflits avec `Serial`.
- Remplacer les `delay()` par une gestion non bloquante.
- Ajouter un réglage mémorisé de la position avec un potentiomètre de retour.

## Auteurs

- Reda Haddar
- Kawthar Derouich
- Mohammed Laaouar

Encadrant : M. Azougagh Houssine.

## Licence

Projet pédagogique. Licence MIT, voir le fichier `LICENSE` (à ajouter).
