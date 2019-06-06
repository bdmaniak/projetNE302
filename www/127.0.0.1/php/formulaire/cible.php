<!DOCTYPE html>
<html>
    <head>
        <title>Notre premier formulaire</title>
        <meta charset="utf-8" />
    </head>
    <body>
        <h2>Bienvenue <?php echo htmlspecialchars($_POST['pseudo']) ?> !</h2>
        
        <p>
            Ton mot de passe est <?php echo htmlspecialchars($_POST['mdp']) ?>. <br />
            Et oui, tu pensais vraiment être en sécurité ? ;)
        </p>
    </body>
</html>