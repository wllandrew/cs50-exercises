select
    name
from
    people
join stars on stars.person_id = people.id
join movies on movies.id = stars.movie_id
where
    title in
    (
        select
            title
        from
            movies
        join stars on stars.movie_id = movies.id
        join people on people.id = stars.person_id
        where
            name = 'Kevin Bacon'
        and
            birth = '1958'
    )
and
    name != 'Kevin Bacon';
