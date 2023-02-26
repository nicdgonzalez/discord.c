discord.c (for Windows)
========================

.. contents:: Table of Contents

Introduction
-------------

A wrapper for the Discord API written in C.

The goal of this project was to create a wrapper for the Discord API written
with as few dependencies as possible and to cover the entire Discord Library.


Technologies Used
------------------

- `OpenSSL 3.0 <https://github.com/openssl/openssl>`_

Project Status
---------------

This project is currently in its *early stages of development*.


Version Naming
---------------

This library uses semantic versioning:

.. code:: txt

  MAJOR.MINOR.PATCH

Where an increment in:

* ``MAJOR`` = Incompatible changes (i.e., code may need to be updated).
* ``MINOR`` = Backwards compatible feature changes.
* ``PATCH`` = Backwards compatible bug fixes.


Bug/Feature Request
--------------------

If you find a bug (program failed to run and/or gave undesired results)
or you just want to request a feature, kindly open a new issue
`here <https://github.com/nicdgonzalez/discord.c/issues>`_.


Contributing
-------------

Want to contribute? Great!

To fix a bug or enhance an existing module, follow these steps:

- `Fork <https://github.com/nicdgonzalez/discord.c/fork>`_ the repository and create a new branch.

.. code:: console

  $ git clone "https://github.com/{username}/{respository}.git"
  $ cd {respository}
  $ git checkout -b "improve-feature"

- Make the appropriate changes and stage the modified files.

.. code:: console

  $ git add <changed file(s)>

- Commit the changes.

.. code:: console

  $ git commit -m "Improve feature."

- Push to the new branch.

.. code:: console

  $ git push "origin" "improve-feature"

- Create a `Pull Request <https://github.com/nicdgonzalez/discord.c/pulls>`_.
