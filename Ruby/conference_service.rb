require 'conference_accolade'
require 'conference_accolades'
require 'conference_stats'
require 'conference_team'

class ConferenceService

  def initialize repository, team_service
    @repository   = repository
    @team_service = team_service
  end

  def get conference_id
    conference = @repository.read Conference.new conference_id

    unless conference.nil?
      get_accolades conference
      get_stats     conference
      get_teams     conference
    end

    conference
  end

  def get_accolades conference
    conference_accolade = ConferenceAccolade.new

    results = @repository.custom_read conference_accolade.build_select_by_conference, conference.conference_id

    unless results.nil? or results.length == 0
      conference.accolades = []

      results.each do |result|
        conference_accolade.from_hash result

        conference.accolades.push conference_accolade

        conference_accolade = ConferenceAccolade.new
      end
    end
  end

  def get_stats conference
    conference_stats = ConferenceStats.new

    results = @repository.custom_read conference_stats.build_select_by_conference, conference.conference_id

    unless results.nil? or results.length == 0
      conference.stats = []

      results.each do |result|
        conference_stats.from_hash result

        conference.stats.push conference_stats

        conference_stats = ConferenceStats.new
      end
    end
  end

  def get_teams conference
    conference_teams = ConferenceTeam.new

    results = @repository.custom_read conference_teams.build_select_by_conference, conference.conference_id

    unless results.nil? or results.length == 0
      conference.teams = []

      results.each do |result|
        conference_teams.from_hash result

        unless conference_teams.team_id.nil?
          conference_teams.team = @team_service.get conference_teams.team_id
        end

        conference.teams.push conference_teams

        conference_teams = ConferenceTeam.new
      end
    end
  end

  def save conference
    save_accolades        conference.accolades
    save_conference_stats conference.stats
    save_teams            conference.teams

    @repository.create conference
  end

  def save_accolades accolades
    return if accolades.nil?

    if accolades.is_a? Array
      accolades.each do |accolade|
        @repository.create accolade
      end
    else
      @repository.create accolades
    end
  end

  def save_conference_stats conference_stats
    return if conference_stats.nil?

    if conference_stats.is_a? Array
      conference_stats.each do |stats|
        @repository.create stats
      end
    else
      @repository.create conference_stats
    end
  end

  def save_teams teams
    return if teams.nil?

    if teams.is_a? Array
      teams.each do |team|
        save_team team
      end
    else
      save_team teams
    end
  end

  def save_team team
    return if team.nil?

    @repository.create team

    unless team.team.nil?
      @team_service.save team.team
    end
  end
end
