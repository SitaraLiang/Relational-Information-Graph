# Relational-Information-Graph
Course project of "Structures de données et algorithmes II" - Unistra L2S4

**[⭐️ Version Francais ⭐️](#cadre-du-problème)**

**[⭐️ English Version ⭐️](#frame-of-the-problem)**


## Cadre du problème
### Graphe d’information relationnelle
Dans le cadre d’un outil d’aide à l’enquête criminelle, on se propose de modéliser des relations entre « entités ». Ces entités sont des objets, des lieux et des individus. La figure 1 ci-dessous présente graphiquement l’information relationnelle que l’on souhaite modéliser.

Chaque entité a un nom (chaine de caractères) et un type. Le nom de l’entité est unique. On se limite à quatre types différents d’entités : Personne, Objet, Adresse et Ville. Un nom NULL est utilisé comme . L’ensemble des entités qui sont en relation forme un graphe. Chaque relation est également étiquetée (voir figure 1). Il ne peut y avoir qu’une seule relation entre deux mêmes entités, si bien qu’il s’agit d’un graphe simple. Par ailleurs, le graphe est non- orienté et il est valué.
Les types de relations sont définies sous la forme d’une énumération :


## Frame of the problem
### Relational information graph
As part of a criminal investigation aid tool, we propose to model relationships between “entities”. These entities are objects, places and individuals. Figure 1 below graphically presents the relational information that we wish to model.

Each entity has a name (character string) and a type. The entity name is unique. We limit ourselves to four different types of entities: Person, Object, Address and City. A NULL name is used like . The set of entities that are related forms a graph. Each relationship is also labeled (see Figure 1). There can only be one relationship between the same two entities, so it is a simple graph. Furthermore, the graph is undirected and it is valued.
The relationship types are defined in the form of an enumeration:
