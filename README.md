#defis-serveur-http

Ce repo contient le code source d'un mini serveur pour le défi d'un forum de programmation informatique.

Le but était de programmer à la main (sans utiliser de bibliothèques tierces) un serveur web capable d'afficher une liste de compteurs, les incrémenter, de pouvoir les afficher, en créer et en supprimer.

Au début, deux compteurs existent par défaut :
- le compteur carotte avec une valeur de 10
- le compteur etoile qui contient la somme de tous les autres compteurs

Voici le texte original:

**Le serveur**

Le serveur tourne en HTTP/1.0 derrière le port 8080 et est compatible avec les principaux navigateurs. Le serveur comprend les requêtes HEAD, GET, POST, PUT et DELETE. Le serveur expose une application "compteur" à l'url http://compteur.notaname.fr/, les autres url doivent renvoyer une page d'erreur au format HTML avec un code 404. Le serveur doit être capable de répondre à plusieurs utilisateurs à la fois. Le serveur peut être accessible à l'extérieur de votre réseau domestique. Le serveur peut comprendre TLS, dans quel cas il écoute sur le port 8443.

Parce que le but de ce défi est d'implémenter un serveur web, il est bien entendu interdit d'utiliser un quelconque framework. Plus précisément, vous n'avez pas le droit d'utiliser une quelconque bibliothèque HTTP et vous vous contentez des bibliothèques réseaux et systèmes de votre langage. Le choix de votre stack système est libre, vous pouvez faire du multithreading, de l'asynchrone, etc.

**L'application**

L'application permet de manipuler des compteurs. Le serveur démarre avec deux compteurs : "carotte" et "etoile". Le compteur "carotte" est normal et est initialisé à 10. Un compteur "etoile" est le seul compteur spécial, il n'est pas possible de le modifier et il renvoie la somme de tous les autres compteurs. La valeur d'un compteur ne peut que croître.
L'application compteur admet 5 routes (en plus de HEAD):

* GET / : retourne la liste de tous les compteurs.
* GET /<compteur> : retourne l'état du compteur <compteur>.
* POST / : crée un nouveau compteur et le renvoie.
* PUT /<compteur> : met à jour la valeur du compteur <compteur>.
* DELETE /<compteur> : supprime le compteur <compteur>.

Il est possible de communiquer en x-www-form-urlencoded et en JSON avec l'application (requête). L'application est capable de répondre au format HTML, au format JSON ou à défaut au format texte (réponse). Interessez vous aux headers Accept et Content-Type.

Pour simuler une application plus compliquée et mettre à mal votre implémentation réseau, chaque route simule des calculs et des requêtes à un service externe. Vous simulez les calculs en faisant tourner une boucle dans le vide à raison de 0.2 secondes. Vous simulez une requête externe en mettant votre processus en sleep pendant 0.3 secondes au milieu.

(*note: ce comportement à été supprimé dans le code source de ce repos*)

**Considérations techniques**

Pour faciliter l'implémentation, vous pouvez limiter la taille de la commande et des headers à maximum 1024 caractères par ligne. La RFC (de ce qu'on sait) ne précise rien à ce propos et la plupart des serveurs permettent 8k ou 16k caractères par ligne dans les headers.

**Ressources**

Voici une liste de ressource que nous vous conseillons de lire. Nous ajouterons à cette liste toutes les ressources que vous nous conseillerez.

* https://fr.wikipedia.org/wiki/Hypertext_Transfer_Protocol
* https://www.pierre-giraud.com/http-reseau-securite-cours/
* http://www.kegel.com/c10k.html
* https://www.w3.org/Protocols/HTTP/1.0/spec.html