import pandas as pd
from sklearn.ensemble import RandomForestClassifier
from sklearn.model_selection import train_test_split
from sklearn.metrics import classification_report, accuracy_score
import matplotlib.pyplot as plt
from sklearn.impute import SimpleImputer

data = pd.read_csv('../data/ours/fall.csv')

# Suppression de la première ligne (si elle contient des noms de colonnes supplémentaires)
data = data.iloc[1:].reset_index(drop=True)

X = data.drop(columns=['label'])  # Caractéristiques
y = data['label']  # Étiquettes

# 3. Division des données en ensemble d'entraînement (70%) et de test (30%)
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.3, random_state=42)

# 2. Création et entraînement du modèle RandomForest
random_forest_model = RandomForestClassifier(n_estimators=100, max_depth=None, random_state=42)

# Entraînement du modèle
random_forest_model.fit(X_train, y_train)

# 3. Évaluation du modèle
# Prédictions sur les données de test
y_pred = random_forest_model.predict(X_test)

# Calcul et affichage de l'accuracy
accuracy = accuracy_score(y_test, y_pred)
print("Accuracy :", accuracy)

# Affichage d'un rapport de classification détaillé avec zero_division
print("\nRapport de classification :\n")
print(classification_report(y_test, y_pred, zero_division=0))

# 4. Visualisation de l'importance des caractéristiques
feature_importances = random_forest_model.feature_importances_

# Affichage sous forme de barres
plt.figure(figsize=(10, 6))
plt.bar(range(len(feature_importances)), feature_importances, color='skyblue')
plt.xlabel('Caractéristiques')
plt.ylabel('Importance')
plt.title('Importance des caractéristiques dans RandomForest')
plt.show()