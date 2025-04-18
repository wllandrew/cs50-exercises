import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():

    portfolio = db.execute("SELECT symbol, quantity FROM portfolio WHERE user_id = ?", session["user_id"])

    total_stock_value = 0
    for pair in portfolio:
        pair["current_price"] = lookup(pair["symbol"])["price"]
        pair["total"] = float(pair["current_price"]) * int(pair["quantity"])
        total_stock_value += pair["total"]

    balance = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]["cash"]

    total = float(total_stock_value) + float(balance)

    return render_template("index.html", portfolio=portfolio, total_stock_value=total_stock_value, balance=balance, total=total)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():

    if request.method == "POST":

        if not request.form.get("symbol") or not request.form.get("shares"):
            return apology("Invalid input")

        shares = request.form.get("shares")

        if not shares.isdigit() or int(shares) <= 0:
            return apology("Invalid number of shares")

        shares = int(shares)
        data = lookup(request.form.get("symbol"))

        if data:

            price = data["price"]
            total_price = price * shares
            balance = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]["cash"]

            if total_price > balance:
                return apology("Not enough balance")

            balance = balance - total_price
            db.execute("UPDATE users SET cash = ? WHERE id = ?", balance, session["user_id"])

            db.execute("INSERT INTO transactions(user_id, symbol, price, shares, type) VALUES (?, ?, ?, ?, ?)",
                       session["user_id"], data["symbol"], data["price"], shares, "BUY")


            current = db.execute("SELECT quantity FROM portfolio WHERE symbol = ? AND user_id = ?",
                                 data["symbol"], session["user_id"])

            if current:
                current_quantity = current[0]["quantity"]
                db.execute("UPDATE portfolio SET quantity = ? WHERE user_id = ?", current_quantity + shares, session["user_id"])
            else:
                db.execute("INSERT INTO portfolio(user_id, symbol, quantity) VALUES (?, ?, ?)", session["user_id"], data["symbol"], shares)

            return redirect("/")

        else:
            return apology("Error while fetching data")
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    historic = db.execute("SELECT symbol, price, shares, type FROM transactions WHERE user_id = ?", session["user_id"])
    return render_template("history.html", historic=historic)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    if request.method == "POST":
        if not request.form.get("symbol"):
            return apology("Symbol is required", 400)

        data = lookup(request.form.get("symbol"))

        if data:
            return render_template("quote.html", data=data)
        else:
            return apology("Unable to fetch data", 400)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():

    session.clear()

    if request.method == "POST":
        # Necessary checks
        if not request.form.get("username"):
            return apology("Username is required", 400)

        elif not request.form.get("password") or not request.form.get("confirmation"):
            return apology("Password is required", 400)

        elif request.form.get("password") != request.form.get("confirmation"):
            return apology("Confirmation does not match", 400)

        elif db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username")):
            return apology("Username already taken", 400)

        hash = generate_password_hash(request.form.get("password"))
        username = request.form.get("username")

        db.execute("INSERT INTO users(username, hash) VALUES (?, ?)", username, hash)

        session["user_id"] = db.execute("SELECT id FROM users WHERE username = ?", username)[0]["id"]

        return redirect("/")

    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():

    if request.method == "POST":

        if not request.form.get("shares") or not request.form.get("symbol"):
            return apology("Invalid input", 400)

        shares = request.form.get("shares")

        if not shares.isdigit() or int(shares) < 1:
            return apology("Invalid share number", 400)

        shares = int(shares)
        data = lookup(request.form.get("symbol"))

        if data:

            portfolio = db.execute("SELECT quantity FROM portfolio WHERE user_id = ? AND symbol = ?",
                                   session["user_id"], data["symbol"])[0]["quantity"]
            diff = portfolio - shares

            if diff < 0:
                return apology("Cannot realize sell", 400)
            elif diff == 0:
                db.execute("DELETE FROM portfolio WHERE user_id = ? AND symbol = ?", session["user_id"], data["symbol"])
            else:
                db.execute("UPDATE portfolio SET quantity = ? WHERE symbol = ? AND user_id = ?", diff, data["symbol"], session["user_id"])

            db.execute("INSERT INTO transactions(user_id, symbol, price, shares, type) VALUES (?, ?, ?, ?, ?)",
                       session["user_id"], data["symbol"], data["price"], shares, "SELL")

            value = shares * data["price"]
            current_quantity = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]["cash"]
            db.execute("UPDATE users SET cash = ? WHERE id = ?", current_quantity + value, session["user_id"])

            return redirect("/")
        else:
            return apology("Error while fetching data", 501)

    else:
        portfolio = db.execute("SELECT symbol FROM portfolio WHERE user_id = ?", session["user_id"])
        return render_template("sell.html", portfolio=portfolio)

@app.route("/password", methods=["GET", "POST"])
@login_required
def password_change():

    if request.method == "POST":

        if not request.form.get("password"):
            return apology("Invalid password", 403)

        pass_hash = db.execute("SELECT hash FROM users WHERE id = ?", session["user_id"])

        if check_password_hash(pass_hash[0]["hash"], request.form.get("password")):
            return redirect("/change")

        else:
            return apology("Wrong password", 401)

    else:
        return render_template("password.html")

@app.route("/change", methods=["GET", "POST"])
@login_required
def change():
    if request.method == "POST":

        if not request.form.get("password") or not request.form.get("password2"):
            return apology("Invalid password", 403)

        pass1 = request.form.get("password")
        pass2 = request.form.get("password2")

        if pass1 != pass2:
            return apology("The passwords does not match", 403)

        db.execute("UPDATE users SET hash = ? WHERE id = ?", generate_password_hash(pass1), session["user_id"])
        return redirect("/")

    else:
        return render_template("change.html")
