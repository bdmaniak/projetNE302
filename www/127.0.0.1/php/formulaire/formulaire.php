<!DOCTYPE html>
<html>
    <head>
        <title>Notre premier formulaire</title>
        <meta charset="utf-8" />
    </head>
    <body>
        <h2>Affichage d'un formulaire</h2>
        
        <form method="post" action="cible.php">
            <p>
                Pseudo : <input type="text" name="pseudo" value="Pseudo" />
            </p>
            <p>
                Mot de passe : <input type="password" name="mdp" value="motdepasse" /> <br /><br />
                <input type="submit" value="Valider" />
            </p>
        </form>
    </body>
</html>